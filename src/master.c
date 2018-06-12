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

int
bk_master_init(bk_master_t* master, const uint32_t nb_workers) {
    assert(master->id == 0);  // already initiliazed?
    master->id = -1;

    master->nb_workers = 0;
    master->_workers = calloc(sizeof(bk_worker_t), nb_workers);
    assert(master->_workers);
    for (uint32_t i = 0; i < nb_workers; i++) {
        BK_RETERR(bk_worker_init(master->_workers + i, i + 1));
        master->nb_workers++;
    }

    return 0;
}

void
bk_master_fini(bk_master_t* master) {
    for (uint32_t i = 0; i < master->nb_workers; i++) {
        bk_worker_t* worker = master->_workers + i;
        bk_worker_fini(worker);
    }
    free(master->_workers);

    memset(master, 0, sizeof(*master));
}

// -----------------------------------------------------------------------------

void
_bk_master_signal_cb(uv_signal_t* handle, int signum) {
    (void)handle;
    (void)signum;

    log_info("caught SIGINT, shutting down...");
    uv_stop(handle->loop);
}

void
_bk_client_close_cb(uv_handle_t* client) {
    free(client);
}

void
_bk_master_listen_cb(uv_stream_t* server, int err) {
    BK_ASSERT(err);  // TODO(cmc)

    log_info("new conn!");

    uv_loop_t* loop = ((uv_tcp_t*)server)->loop;

    uv_tcp_t* client = (uv_tcp_t*)calloc(sizeof(uv_tcp_t), 1);
    BK_ASSERT(uv_tcp_init(loop, client));

    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_close((uv_handle_t*)client, _bk_client_close_cb);
    }
}

// TODO(cmc): real graceful shutdown
int
bk_master_run(bk_master_t*           master,
              const struct sockaddr* laddr,
              const uint32_t         backlog_size) {
    (void)master;

    uv_loop_t loop;
    BK_RETERR(uv_loop_init(&loop));

    uv_signal_t sighandler;
    BK_RETERR(uv_signal_init(&loop, &sighandler));
    BK_RETERR(
        uv_signal_start_oneshot(&sighandler, _bk_master_signal_cb, SIGINT));

    uv_tcp_t listener;
    BK_RETERR(uv_tcp_init(&loop, &listener));
    BK_RETERR(uv_tcp_bind(&listener, laddr, 0));
    BK_RETERR(uv_tcp_nodelay(&listener, 1));
    BK_RETERR(
        uv_listen((uv_stream_t*)&listener, backlog_size, _bk_master_listen_cb));

    char ip[16] = {0};
    uv_ip4_name((const struct sockaddr_in*)laddr, ip, sizeof(ip));
    log_info("master server now running on %s", ip);

    int err = uv_run(&loop, UV_RUN_DEFAULT);
    log_info("master server shutting down...");

    // 1. stop accepting signals
    uv_close((uv_handle_t*)&sighandler, NULL);
    // 2. stop accepting connection
    uv_close((uv_handle_t*)&listener, NULL);
    // 3. wait for existing streams to end
    do {
        // TODO(cmc): timer / hard deadline
        err = uv_run(&loop, UV_RUN_DEFAULT);
    } while (err);
    log_info("master server shutdown complete");

    /* uv_close((uv_handle_t*)&sighandler, NULL); */
    /* BK_LOGERR(uv_close(&sighandler, NULL)); */
    BK_LOGERR(uv_loop_close(&loop));

    return err;
}
