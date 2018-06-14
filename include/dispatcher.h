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

#pragma once

#include <stdint.h>

#include "worker.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

// _ denotes read-only members (from an external standpoint).
typedef struct bk_dispatcher_s {
    uint32_t _id;

    uint32_t     _nb_workers;
    bk_worker_t* _workers;

    uv_loop_t* _loop;
} bk_dispatcher_t;

void bk_dispatcher_init(bk_dispatcher_t* dispatcher,
                        uint32_t         id,
                        uint32_t         nb_workers,
                        bk_worker_t*     workers);
void bk_dispatcher_fini(bk_dispatcher_t* dispatcher);

void bk_dispatcher_run(void* dispatcher_ptr);
int  bk_dispatcher_handoff(bk_dispatcher_t* dispatcher, uv_os_fd_t client_fd);
