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
#include <unistd.h>

#include "macros.h"
#include "worker.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

int bk_worker_init(bk_worker_t* worker, uint32_t id) {
    (void)worker;
    (void)id;
#ifdef false
    uv_loop_t* loop = calloc(sizeof(uv_loop_t), 1);
    BK_RETERR(uv_loop_init(loop));

    uv_tcp_t listener;
    BK_RETERR(uv_tcp_init(loop, &listener));

    struct sockaddr_in addr;
    BK_RETERR(uv_ip4_addr(laddr, port, &addr));

    BK_RETERR(uv_tcp_bind(&listener, (const struct sockaddr*)&addr, 0));
    BK_RETERR(uv_listen((uv_stream_t*)&listener, 42, NULL));

    BK_RETERR(uv_run(loop, UV_RUN_DEFAULT));

    uv_loop_close(loop);
    free(loop);
#endif

    return 0;
}

void bk_worker_fini(bk_worker_t* worker) { (void)worker; }
