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

#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>

#include "worker.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

typedef struct bk_master {
    uint32_t     id;  // always -1 once initialized
    uint32_t     nb_workers;
    bk_worker_t* _workers;
} bk_master_t;

int  bk_master_init(bk_master_t* master, const uint32_t nb_workers);
void bk_master_fini(bk_master_t* master);

int bk_master_run(bk_master_t*           master,
                  const struct sockaddr* laddr,
                  const uint32_t         backlog_size);
