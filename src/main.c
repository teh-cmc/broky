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
#include <signal.h>
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

static volatile bool _shutdown = false;
static uv_cond_t _shutdown_cond;   // never destroyed
static uv_mutex_t _shutdown_lock;  // never destroyed

static void _sigint_handler(int sig) {
    (void)sig;

    log_info("caught SIGINT");

    uv_mutex_lock(&_shutdown_lock);
    _shutdown = true;
    uv_cond_signal(&_shutdown_cond);
    uv_mutex_unlock(&_shutdown_lock);
}

static void _sigint_watch(void* master) {
    master = (bk_master_t*)master;

    BK_ASSERT(uv_cond_init(&_shutdown_cond));
    BK_ASSERT(uv_mutex_init(&_shutdown_lock));

    bool shutdown = false;
    for (;;) {
        uv_mutex_lock(&_shutdown_lock);
        uv_cond_wait(&_shutdown_cond, &_shutdown_lock);
        shutdown = _shutdown;
        uv_mutex_unlock(&_shutdown_lock);

        if (shutdown) {
            log_info("shutting down");
            bk_master_stop(master);
            return;
        }
    }
}

// -----------------------------------------------------------------------------

int main() {
    uint32_t nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    log_info("cpus: %d", nb_cpus);

    uint32_t nb_workers = nb_cpus - 1;
    bk_worker_t* workers = calloc(sizeof(bk_worker_t), nb_workers);
    for (uint32_t i = 0; i < nb_workers; i++) {
        BK_ASSERT(bk_worker_init(workers + i, i + 1));
    }

    bk_master_t* master = calloc(sizeof(bk_master_t), 1);
    BK_ASSERT(bk_master_init(master, "0.0.0.0", 7070, nb_workers, workers));

    /* uv_thread_t* tids[nb_cpus]; */
    /* for (uintptr_t i = 0; i < nb_cpus; i++) { */
    /*     uv_thread_t tid; */
    /*     BK_ASSERT(uv_thread_create(&tid, new_server, (void*)_servers[i])) */
    /*     tids[i] = &tid; */
    /* } */

    /* for (uint32_t i = 0; i < nb_cpus; i++) { */
    /*     BK_ASSERT(uv_thread_join(tids[i])); */
    /* } */

    uv_thread_t tid;
    BK_ASSERT(uv_thread_create(&tid, _sigint_watch, (void*)master));

    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = _sigint_handler;
    sigaction(SIGINT, &sigact, NULL);

    BK_ASSERT(bk_master_run(master, 42));

    for (uint32_t i = 0; i < nb_workers; i++) {
        bk_worker_t* worker = workers + i;
        bk_worker_fini(worker);
        free(worker);
    }

    bk_master_fini(master);
    free(master);

    return 0;
}
