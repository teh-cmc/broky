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

#include "client/memory.h"
#include "client/tty.h"
#include "common/macros.h"
#include "protocol/protocol.h"

#include "fbs/requests_builder.h"
#include "libuv/uv.h"

// -----------------------------------------------------------------------------

static void
_bk_tty_close_cb(uv_handle_t* tty_raw) {
    bk_tty_t* tty = tty_raw->data;
    assert(tty);

    free(tty->_tty);
    flatcc_builder_clear(tty->_builder);
    free(tty->_builder);
}

static void
_bk_tty_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        free(buf->base);
        uv_close((uv_handle_t*)stream, _bk_tty_close_cb);
        if (nread == UV_EOF) return;
        BK_UV_LOGERR(nread);
    }

    assert(buf->base);
    assert(buf->len);

    bk_tty_t* tty = stream->data;
    assert(tty);

    if (!strncmp(buf->base, "ping", strlen("ping"))) {
        void* buf = NULL;
        assert(!bk_proto_new_req_ping(tty->_builder, &buf));
        flatcc_builder_aligned_free(buf);
    } else {
        log_warn("'%.*s': not a valid command", nread - 1, buf->base);
    }

    free(buf->base);
}

// -----------------------------------------------------------------------------

int
bk_tty_init(bk_tty_t* tty, uv_loop_t* loop) {
    uv_tty_t* tty_raw = calloc(sizeof(*tty_raw), 1);
    BK_RETERR(uv_tty_init(loop, tty_raw, 0, 1));

    flatcc_builder_t* builder = calloc(sizeof(*builder), 1);
    BK_RETERR(flatcc_builder_init(builder));

    tty->_tty = tty_raw;
    tty->_loop = loop;
    tty->_builder = builder;

    tty_raw->data = tty;

    BK_RETERR(uv_read_start(
        (uv_stream_t*)tty_raw, bk_dumb_alloc_cb, _bk_tty_read_cb));

    return 0;
}

void
bk_tty_fini(bk_tty_t* tty) {
    memset(tty, 0, sizeof(*tty));
}
