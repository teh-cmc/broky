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
bk_dispatcher_init(bk_dispatcher_t*   dispatcher,
                   uint32_t           id,
                   const bk_worker_t* workers) {
    dispatcher->id = id;
    dispatcher->_workers = workers;
}

void
bk_dispatcher_fini(bk_dispatcher_t* dispatcher) {
    memset(dispatcher, 0, sizeof(*dispatcher));
}

// -----------------------------------------------------------------------------

// NOTE(cmc): Always run as a thread.
// TODO(cmc): remove assertions and forward errors to listener
void
bk_dispatcher_run(void* dispatcher_ptr) {
    bk_dispatcher_t* dispatcher = dispatcher_ptr;
    (void)dispatcher;

#ifdef false
    uv_loop_t loop;
    BK_ASSERT(uv_loop_init(&loop));

    int err = uv_run(&loop, UV_RUN_DEFAULT);
    log_info("dispatcher %d shutting down...", dispatcher->id);

    // wait for existing streams to end
    do {
        err = uv_run(&loop, UV_RUN_DEFAULT);
    } while (err);
    log_info("dispatcher %d shutdown complete");

    BK_LOGERR(uv_loop_close(&loop));
#endif
}
