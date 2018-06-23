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

#include "server/worker.h"

#include "flatcc/flatcc_builder.h"
#include "libuv/uv.h"

// -----------------------------------------------------------------------------

// _ denotes read-only members (from an external standpoint).
typedef struct bk_tty_s {
    uint32_t _id;

    uv_tty_t*  _tty;
    uv_loop_t* _loop;

    flatcc_builder_t* _builder;
} bk_tty_t;

int  bk_tty_init(bk_tty_t* tty, uv_loop_t* loop);
void bk_tty_fini(bk_tty_t* tty);
