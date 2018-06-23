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

#include <stdlib.h>

#include "common/macros.h"

#include "libuv/uv.h"

// -----------------------------------------------------------------------------

void
bk_stream_write_cb(uv_write_t* req, int status) {
    BK_UV_LOGERR(status);
    free(req);
}

void
bk_stream_close_cb(uv_handle_t* client) {
    free(client);
}
