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

#pragma once

#include "log.h"

// -----------------------------------------------------------------------------

// TODO(cmc): likely/unlikely

#define BK_ASSERT(uvexpr)                 \
    if (uvexpr < 0) {                      \
        fprintf(stderr,                    \
                "%s:%d error: %s\n\t%s\n", \
                __FILE__,                  \
                __LINE__,                  \
                uv_strerror(uvexpr),       \
                #uvexpr);                  \
        abort();                           \
    }

#define BK_LOGERR(uvexpr)              \
    if (uvexpr < 0) {                   \
        log_error(uv_strerror(uvexpr)); \
    }

#define BK_RETERR(expr) \
    do {                 \
        int err = expr;  \
        if (err < 0) {   \
            return err;  \
        }                \
    } while (0);
