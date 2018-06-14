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

#include "dispatcher.h"
#include "macros.h"
#include "memory.h"
#include "stream.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

void
bk_dispatcher_init(bk_dispatcher_t* dispatcher,
                   uint32_t         id,
                   uint32_t         nb_workers,
                   bk_worker_t*     workers) {
    dispatcher->_id = id;

    dispatcher->_nb_workers = nb_workers;
    dispatcher->_workers = workers;
}

void
bk_dispatcher_fini(bk_dispatcher_t* dispatcher) {
    memset(dispatcher, 0, sizeof(*dispatcher));
}

// -----------------------------------------------------------------------------

static void
_bk_dispatcher_timer_cb(uv_timer_t* timer) {
    (void)timer;
}

static int
_bk_dispatcher_announce(bk_dispatcher_t* dispatcher,
                        uv_stream_t*     stream,
                        const char*      message,
                        const uint32_t   message_len) {
    char* msg = calloc(sizeof(*msg), message_len + 32);
    BK_RETERR(sprintf(msg, "dispatcher #%u: %s", dispatcher->_id, message));

    uv_buf_t    buf2[] = {{.base = msg, .len = strlen(msg)}};
    uv_write_t* req = calloc(sizeof(*req), 1);
    assert(req);
    BK_RETERR(uv_write(req, stream, buf2, 1, bk_stream_write_cb));

    return 0;
}

static void
_bk_dispatcher_read_cb(uv_stream_t*    stream,
                       ssize_t         nread,
                       const uv_buf_t* buf) {
    if (nread < 0) {
        if (nread == UV_EOF) {
            BK_ASSERT(uv_read_stop(stream));
            return;
        }
        BK_LOGERR(nread);
        uv_close((uv_handle_t*)stream, bk_stream_close_cb);
    }

    assert(buf->base);
    assert(buf->len);

    bk_dispatcher_t* dispatcher = stream->data;

    BK_ASSERT(_bk_dispatcher_announce(dispatcher, stream, buf->base, buf->len));
    free(buf->base);
}

// -----------------------------------------------------------------------------

// TODO(cmc): remove assertions and forward errors to listener
void
bk_dispatcher_run(void* dispatcher_ptr) {
    bk_dispatcher_t* dispatcher = dispatcher_ptr;

    uv_loop_t loop;
    BK_ASSERT(uv_loop_init(&loop));

    uv_timer_t timer;
    BK_ASSERT(uv_timer_init(&loop, &timer));
    BK_ASSERT(uv_timer_start(&timer, _bk_dispatcher_timer_cb, 1000, 1000));

    dispatcher->_loop = &loop;
    int err = uv_run(&loop, UV_RUN_DEFAULT);
    dispatcher->_loop = NULL;  // racy
    log_info("dispatcher %u shutting down...", dispatcher->_id);

    // wait for existing streams to end
    do {
        err = uv_run(&loop, UV_RUN_DEFAULT);
    } while (err);
    log_info("dispatcher %u shutdown complete", dispatcher->_id);

    BK_LOGERR(uv_loop_close(&loop));
}

int
bk_dispatcher_handoff(bk_dispatcher_t* dispatcher, uv_os_fd_t client_fd) {
    uv_tcp_t* client = (uv_tcp_t*)calloc(sizeof(*client), 1);
    assert(client);
    client->data = dispatcher;
    BK_RETERR(uv_tcp_init(dispatcher->_loop, client));
    BK_RETERR(uv_tcp_nodelay(client, 1));
    BK_RETERR(uv_tcp_open(client, client_fd));

    char* ann = "you're being moved over to dispatcher #%u\n";
    char* msg = calloc(sizeof(*msg), strlen(ann) + 16);
    BK_RETERR(sprintf(msg, ann, dispatcher->_id));

    uv_buf_t    buf[] = {{.base = msg, .len = strlen(msg)}};
    uv_write_t* req = calloc(sizeof(*req), 1);
    assert(req);
    BK_RETERR(uv_write(req, (uv_stream_t*)client, buf, 1, bk_stream_write_cb));

    BK_RETERR(uv_read_start(
        (uv_stream_t*)client, bk_dumb_alloc_cb, _bk_dispatcher_read_cb));

    return 0;
}
