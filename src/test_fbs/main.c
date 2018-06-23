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

#include "fbs/message_builder.h"

// -----------------------------------------------------------------------------
//
#define BK_ASSERT(uvexpr)                                                    \
    if (uvexpr < 0) {                                                        \
        fprintf(stderr, "%s:%d error: %s\n\n", __FILE__, __LINE__, #uvexpr); \
        abort();                                                             \
    }

int
main() {
    flatcc_builder_t builder, *B = &builder;

    BK_ASSERT(flatcc_builder_init(B));

    flatbuffers_string_ref_t payload =
        flatbuffers_string_create_str(B, "coucou les loulous");
    BK_ASSERT(bk_fbs_Message_start_as_root(B));
    bk_fbs_Message_payload_add(B, payload);
    bk_fbs_Message_end_as_root(B);

    size_t size;
    void*  buf = flatcc_builder_finalize_aligned_buffer(B, &size);

    printf("size=%lu: '%.*s'\n", size, (int)size, (char*)buf);

    bk_fbs_Message_table_t msg = bk_fbs_Message_as_root(buf);

    flatbuffers_string_t name = bk_fbs_Message_payload(msg);
    size_t               name_len = flatbuffers_string_len(name);

    printf("size=%lu: '%.*s'\n", name_len, (int)name_len, (char*)name);

    free(buf);

    flatcc_builder_clear(B);
}
