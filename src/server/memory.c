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
#include <stdlib.h>

#include "libuv/uv.h"

// -----------------------------------------------------------------------------

void
bk_dumb_alloc_cb(uv_handle_t* handle, size_t hint, uv_buf_t* buf) {
    (void)handle;
    (void)hint;

    // TODO(cmc): try with 1 byte and see that happens protocol-wise.
    buf->base = calloc(4 * 1024, 1);
    assert(buf->base);
    buf->len = 4 * 1024;
}
