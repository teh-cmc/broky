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
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "macros.h"
#include "master.h"
#include "worker.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

int bk_master_init(bk_master_t* master, const char* laddr, uint16_t port,
                   uint32_t nb_workers, bk_worker_t* workers) {
    assert(master->id == 0);  // already initiliazed?

    master->nb_workers = nb_workers;
    master->_workers = workers;

    master->loop = calloc(sizeof(uv_loop_t), 1);
    BK_RETERR(uv_loop_init(master->loop));

    // TODO(cmc): IPv6 support
    master->addr = calloc(sizeof(struct sockaddr), 1);
    BK_RETERR(uv_ip4_addr(laddr, port, (struct sockaddr_in*)master->addr));

    return 0;
}

void bk_master_fini(bk_master_t* master) {
    BK_LOGERR(uv_loop_close(master->loop));

    free(master->addr);
    free(master->loop);

    memset(master, 0, sizeof(*master));
}

// -----------------------------------------------------------------------------

void _bk_master_listen_cb(uv_stream_t* server, int err) {
    BK_ASSERT(err);  // TODO(cmc)

    log_info("new conn!");

    uv_loop_t* loop = ((uv_tcp_t*)server)->loop;

    uv_tcp_t* client = (uv_tcp_t*)calloc(sizeof(uv_tcp_t), 1);
    BK_ASSERT(uv_tcp_init(loop, client));

    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_close((uv_handle_t*)client, NULL);  // frees the client!
    }
}

int bk_master_run(bk_master_t* master, uint32_t backlog) {
    uv_tcp_t listener;
    BK_RETERR(uv_tcp_init(master->loop, &listener));

    BK_RETERR(uv_tcp_bind(&listener, (const struct sockaddr*)master->addr, 0));
    BK_RETERR(
        uv_listen((uv_stream_t*)&listener, backlog, _bk_master_listen_cb));

    char ip[16] = {0};
    uv_ip4_name((const struct sockaddr_in*)master->addr, ip, sizeof(ip));
    log_info("master server running on %s", ip);
    BK_RETERR(uv_run(master->loop, UV_RUN_DEFAULT));

    return 0;
}

void bk_master_stop(bk_master_t* master) { uv_stop(master->loop); }
