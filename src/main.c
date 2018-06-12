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

#include "libuv/include/uv.h"

// -----------------------------------------------------------------------------

int
main() {
    uint32_t nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    log_info("detected %u CPU cores", nb_cpus);

    uint32_t nb_workers = nb_cpus < 2 ? 1 : nb_cpus - 1;
    log_info("booting 1 master + %u worker(s)", nb_workers);

    // TODO(cmc): IPv6 support
    // TODO(cmc): argv
    struct sockaddr_in* addr = calloc(sizeof(struct sockaddr_in), 1);
    assert(addr);
    BK_ASSERT(uv_ip4_addr("0.0.0.0", 7070, addr));

    bk_master_t* master = calloc(sizeof(bk_master_t), 1);
    BK_ASSERT(bk_master_init(master, nb_workers));

    // TODO(cmc): argv
    BK_LOGERR(bk_master_run(master, (const struct sockaddr*)addr, 42));
    free(addr);

    bk_master_fini(master);
    free(master);

    return 0;
}
