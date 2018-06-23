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

#include "fbs/requests_builder.h"
#include "protocol/protocol.h"

#include "logc/log.h"

// -----------------------------------------------------------------------------

int
main(void) {
    flatcc_builder_t builder, *B = &builder;

    assert(!flatcc_builder_init(B));

    void *buf = NULL;
    assert(!bk_proto_new_req_ping(B, &buf));
    flatcc_builder_aligned_free(buf);

    flatcc_builder_clear(B);

    return 0;
}
