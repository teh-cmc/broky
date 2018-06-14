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
#include <unistd.h>

#include "dispatcher.h"
#include "listener.h"
#include "log.h"
#include "macros.h"
#include "math.h"
#include "worker.h"

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

int
main() {
    uint32_t nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    log_info("detected %u CPU cores", nb_cpus);

    const uint32_t nb_listeners = 1;
    const uint32_t nb_cpus_avail = bk_max(nb_cpus - nb_listeners, 1);
    const uint32_t nb_dispatchers = bk_max(nb_cpus_avail / 4, 1);
    const uint32_t nb_workers = bk_max(nb_cpus_avail - nb_dispatchers, 1);

    bk_worker_t* workers;
    uv_thread_t  worker_tids[nb_workers];
    { /* allocate, initialize & start workers */
        log_info("booting %u worker(s)", nb_workers);

        workers = calloc(sizeof(*workers), nb_workers);
        assert(workers);
        for (uint32_t i = 0; i < nb_workers; i++) {
            bk_worker_t* worker = workers + i;
            bk_worker_init(worker, i);
            BK_ASSERT(uv_thread_create(
                worker_tids + i, bk_worker_run, (void*)worker));
        }
    }

    bk_dispatcher_t* dispatchers;
    uv_thread_t      dispatcher_tids[nb_dispatchers];
    { /* allocate, initialize & start dispatchers */
        log_info("booting %u dispatcher(s)", nb_dispatchers);

        dispatchers = calloc(sizeof(*dispatchers), nb_dispatchers);
        assert(dispatchers);
        for (uint32_t i = 0; i < nb_dispatchers; i++) {
            bk_dispatcher_t* dispatcher = dispatchers + i;
            bk_dispatcher_init(dispatcher, i, workers);
            BK_ASSERT(uv_thread_create(
                dispatcher_tids + i, bk_dispatcher_run, (void*)dispatcher));
        }
    }

    bk_listener_t* listener;
    uv_thread_t    listener_tid;
    { /* allocate, initialize & start listener */
        log_info("booting listener");

        // TODO(cmc): IPv6 support
        struct sockaddr_in* laddr = calloc(sizeof(*laddr), 1);
        assert(laddr);
        // TODO(cmc): argv
        BK_ASSERT(uv_ip4_addr("0.0.0.0", 7070, laddr));

        listener = calloc(sizeof(*listener), 1);
        assert(listener);
        bk_listener_init(listener, (sockaddr_t*)laddr, 42, dispatchers);

        BK_ASSERT(
            uv_thread_create(&listener_tid, bk_listener_run, (void*)listener));
    }

    { /* join listener thread & clean-up resources */
        log_info("cleaning up listener");

        BK_LOGERR(uv_thread_join(&listener_tid));
        bk_listener_fini(listener);

        free(listener);
    }

    { /* join dispatcher threads & clean-up resources */
        log_info("cleaning up %u dispatcher(s)", nb_dispatchers);

        for (uint32_t i = 0; i < nb_dispatchers; i++) {
            bk_dispatcher_t* dispatcher = dispatchers + i;
            BK_LOGERR(uv_thread_join(dispatcher_tids + i));
            bk_dispatcher_fini(dispatcher);
        }

        free(dispatchers);
    }

    { /* join worker threads & clean-up resources */
        log_info("cleaning up %u worker(s)", nb_workers);

        for (uint32_t i = 0; i < nb_workers; i++) {
            bk_worker_t* worker = workers + i;
            BK_LOGERR(uv_thread_join(worker_tids + i));
            bk_worker_fini(worker);
        }

        free(workers);
    }

    return 0;
}
