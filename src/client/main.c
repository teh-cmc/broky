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

#include "client/memory.h"
#include "client/tty.h"
#include "common/macros.h"

#include "libuv/uv.h"
#include "logc/log.h"

// -----------------------------------------------------------------------------

int
main() {
    uv_loop_t loop;
    BK_UV_ASSERT(uv_loop_init(&loop));

    bk_tty_t tty;
    BK_UV_ASSERT(bk_tty_init(&tty, &loop));

    int err = uv_run(&loop, UV_RUN_DEFAULT);
    do {  // wait for existing streams to end
        err = uv_run(&loop, UV_RUN_DEFAULT);
        BK_UV_LOGERR(err)
    } while (err);

    BK_UV_LOGERR(uv_loop_close(&loop));
    BK_UV_LOGERR(uv_tty_reset_mode());

    bk_tty_fini(&tty);
}
