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

void
_bk_dispatcher_timer_cb(uv_timer_t* timer) {
    (void)timer;
}

// -----------------------------------------------------------------------------

// NOTE(cmc): Always run as a thread.
// TODO(cmc): remove assertions and forward errors to
// listener
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
