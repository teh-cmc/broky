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

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

typedef struct bk_worker {
    uint32_t   id;
    uv_loop_t* loop;
} bk_worker_t;

int  bk_worker_init(bk_worker_t* worker, uint32_t id);
void bk_worker_fini(bk_worker_t* worker);
