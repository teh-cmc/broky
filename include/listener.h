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

#include "dispatcher.h"

// -----------------------------------------------------------------------------

typedef struct sockaddr sockaddr_t;

typedef struct bk_listener_s {
    sockaddr_t* laddr;
    uint32_t    backlog_size;

    const bk_dispatcher_t* _dispatchers;
} bk_listener_t;

void bk_listener_init(bk_listener_t*         listener,
                      sockaddr_t*            laddr,
                      const uint32_t         backlog_size,
                      const bk_dispatcher_t* dispatchers);
void bk_listener_fini(bk_listener_t* listener);

void bk_listener_run(void* listener_ptr);
