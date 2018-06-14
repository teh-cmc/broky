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
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dispatcher.h"
#include "listener.h"
#include "log.h"
#include "macros.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

void
bk_listener_init(bk_listener_t*         listener,
                 sockaddr_t*            laddr,
                 uint32_t               backlog_size,
                 const bk_dispatcher_t* dispatchers) {
    listener->laddr = laddr;
    listener->backlog_size = backlog_size;

    listener->_dispatchers = dispatchers;
}

void
bk_listener_fini(bk_listener_t* listener) {
    free(listener->laddr);

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

static void
_bk_client_close_cb(uv_handle_t* client) {
    free(client);
}

static void
_bk_listener_listen_cb(uv_stream_t* server, int err) {
    BK_ASSERT(err);  // TODO(cmc)

    log_info("new conn!");

    uv_loop_t* loop = ((uv_tcp_t*)server)->loop;

    uv_tcp_t* client = (uv_tcp_t*)calloc(sizeof(uv_tcp_t), 1);
    BK_ASSERT(uv_tcp_init(loop, client));

    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_close((uv_handle_t*)client, _bk_client_close_cb);
    }
}

// -----------------------------------------------------------------------------

// TODO(cmc): shouldn't be assert()ing here
void
bk_listener_run(void* listener_ptr) {
    bk_listener_t* listener = listener_ptr;
    assert(listener);

    uv_loop_t loop;
    BK_ASSERT(uv_loop_init(&loop));

    uv_signal_t sighandler;
    BK_ASSERT(uv_signal_init(&loop, &sighandler));
    BK_ASSERT(
        uv_signal_start_oneshot(&sighandler, _bk_listener_signal_cb, SIGINT));

    uv_tcp_t listen_fd;
    BK_ASSERT(uv_tcp_init(&loop, &listen_fd));
    BK_ASSERT(uv_tcp_bind(&listen_fd, listener->laddr, 0));
    BK_ASSERT(uv_tcp_nodelay(&listen_fd, 1));
    BK_ASSERT(uv_listen((uv_stream_t*)&listen_fd,
                        listener->backlog_size,
                        _bk_listener_listen_cb));

    char ip[16] = {0};
    uv_ip4_name((const struct sockaddr_in*)listener->laddr, ip, sizeof(ip));
    log_info("listener server now running on %s", ip);

    int err = uv_run(&loop, UV_RUN_DEFAULT);
    log_info("listener server shutting down...");

    // 1. stop accepting signals
    uv_close((uv_handle_t*)&sighandler, NULL);
    // 2. stop accepting connection
    uv_close((uv_handle_t*)&listen_fd, NULL);
    // 3. wait for existing streams to end
    do {
        // TODO(cmc): timer / hard deadline
        err = uv_run(&loop, UV_RUN_DEFAULT);
    } while (err);

    BK_LOGERR(uv_loop_close(&loop));
    log_info("listener server shutdown complete");
}
