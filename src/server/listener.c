// Copyright © 2018 Clement Rey <cr.rey.clement@gmail.com>.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common/macros.h"
#include "server/dispatcher.h"
#include "server/listener.h"
#include "server/memory.h"
#include "server/stream.h"

#include "libuv/uv.h"
#include "logc/log.h"

// -----------------------------------------------------------------------------

void
bk_listener_init(bk_listener_t*   listener,
                 sockaddr_t*      laddr,
                 uint32_t         backlog_size,
                 const uint32_t   nb_dispatchers,
                 bk_dispatcher_t* dispatchers) {
    listener->_laddr = laddr;
    listener->_backlog_size = backlog_size;

    listener->_nb_dispatchers = nb_dispatchers;
    listener->_dispatchers = dispatchers;
}

void
bk_listener_fini(bk_listener_t* listener) {
    free(listener->_laddr);

    memset(listener, 0, sizeof(*listener));
}

// -----------------------------------------------------------------------------

static void
_bk_listener_signal_cb(uv_signal_t* handle, int signum) {
    (void)handle;
    (void)signum;

    log_info("caught SIGINT, shutting down...");
    uv_stop(handle->loop);
}

static int
_bk_listener_handoff_client(bk_listener_t* listener, uv_tcp_t* client) {
    uint32_t cur_dispatcher = listener->_cur_dispatcher;
    if (++listener->_cur_dispatcher >= listener->_nb_dispatchers) {
        listener->_cur_dispatcher = 0;
    }
    bk_dispatcher_t* dispatcher = listener->_dispatchers + cur_dispatcher;

    uv_os_fd_t client_fd;
    BK_RETERR(uv_fileno((uv_handle_t*)client, &client_fd));
    client_fd = dup(client_fd);
    BK_RETERR(bk_dispatcher_handoff(dispatcher, client_fd));

    return 0;
}

// TODO(cmc): no asserts
static void
_bk_listener_listen_cb(uv_stream_t* server, int err) {
    BK_UV_ASSERT(err);  // TODO(cmc)

    uv_tcp_t* client = (uv_tcp_t*)calloc(sizeof(*client), 1);
    assert(client);
    BK_UV_ASSERT(uv_tcp_init(server->loop, client));
    BK_UV_ASSERT(uv_tcp_nodelay(client, 1));

    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        log_info("accepted new client!");

        BK_UV_ASSERT(_bk_listener_handoff_client(server->data, client));

        uv_close((uv_handle_t*)client, bk_stream_close_cb);
    }
}

// -----------------------------------------------------------------------------

// TODO(cmc): shouldn't be assert()ing here
void
bk_listener_run(void* listener_ptr) {
    bk_listener_t* listener = listener_ptr;
    assert(listener);

    uv_loop_t loop;
    BK_UV_ASSERT(uv_loop_init(&loop));

    uv_signal_t sighandler;
    BK_UV_ASSERT(uv_signal_init(&loop, &sighandler));
    BK_UV_ASSERT(
        uv_signal_start_oneshot(&sighandler, _bk_listener_signal_cb, SIGINT));

    uv_tcp_t listen_sock;
    listen_sock.data = listener;
    BK_UV_ASSERT(uv_tcp_init(&loop, &listen_sock));
    BK_UV_ASSERT(uv_tcp_bind(&listen_sock, listener->_laddr, 0));
    BK_UV_ASSERT(uv_tcp_nodelay(&listen_sock, 1));
    BK_UV_ASSERT(uv_listen((uv_stream_t*)&listen_sock,
                        listener->_backlog_size,
                        _bk_listener_listen_cb));

    char ip[16] = {0};
    uv_ip4_name((const struct sockaddr_in*)listener->_laddr, ip, sizeof(ip));
    log_info("listener server now running on %s", ip);

    int err = uv_run(&loop, UV_RUN_DEFAULT);
    log_info("listener server shutting down...");

    // 1. stop accepting signals
    uv_close((uv_handle_t*)&sighandler, NULL);
    // 2. stop accepting connection
    uv_close((uv_handle_t*)&listen_sock, NULL);
    // 3. wait for existing streams to end
    do {
        // TODO(cmc): timer / hard deadline
        err = uv_run(&loop, UV_RUN_DEFAULT);
    } while (err);

    BK_UV_LOGERR(uv_loop_close(&loop));
    log_info("listener server shutdown complete");

    for (uint32_t i = 0; i < listener->_nb_dispatchers; i++) {
        bk_dispatcher_stop(listener->_dispatchers + i);
    }
}
