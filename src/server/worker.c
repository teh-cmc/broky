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
#include <string.h>

#include "common/macros.h"
#include "server/worker.h"

#include "libuv/uv.h"

// -----------------------------------------------------------------------------

void
bk_worker_init(bk_worker_t* worker, uint32_t id) {
    worker->_id = id;
}

void
bk_worker_fini(bk_worker_t* worker) {
    memset(worker, 0, sizeof(*worker));
}

// -----------------------------------------------------------------------------

void
bk_worker_run(void* worker_ptr) {
    bk_worker_t* worker = worker_ptr;
    (void)worker;

    /* uv_loop_t loop; */
    /* BK_ASSERT(uv_loop_init(&loop)); */

    /* int err = uv_run(&loop, UV_RUN_DEFAULT); */
    /* log_info("worker %d shutting down...", worker->id); */

    /* // wait for existing streams to end */
    /* do { */
    /*     err = uv_run(&loop, UV_RUN_DEFAULT); */
    /* } while (err); */
    /* log_info("worker %d shutdown complete"); */

    /* BK_LOGERR(uv_loop_close(&loop)); */
}
