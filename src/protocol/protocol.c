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

#include <stdio.h>

#include "common/macros.h"
#include "fbs/requests_builder.h"

#include "flatcc/support/hexdump.h"

// -----------------------------------------------------------------------------

int
bk_proto_new_req_ping(flatcc_builder_t* B, void** dst) {
    bk_fbs_Ping_ref_t      ping = bk_fbs_Ping_create(B);
    bk_fbs_Req_union_ref_t req = bk_fbs_Req_as_Ping(ping);

    BK_RETERR(bk_fbs_Request_start_as_root(B));
    BK_RETERR(bk_fbs_Request_req_add(B, req));
    BK_RETERR(bk_fbs_Request_end_as_root(B));

    size_t size;
    void*  buf = flatcc_builder_finalize_aligned_buffer(B, &size);
    assert(buf);

#ifdef DEBUG
    hexdump("Request<Ping>", buf, size, stderr);
#endif

    *dst = buf;

    return 0;
}
