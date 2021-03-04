/*
 * Copyright (C) 2021 Karl Linden <karl.j.linden@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dima/eventually_failing.h>

static void *invoke_eventually_failing(struct dima_proxy *proxy,
                                       const struct dima_invocation *inv) {
    struct dima_eventually_failing *ef
            = (struct dima_eventually_failing *)proxy;
    if (inv->function == DIMA_FREE) {
        return dima_invoke(ef->next, inv);
    } else if (ef->n_allocs == 0) {
        return NULL;
    } else {
        ef->n_allocs--;
        return dima_invoke(ef->next, inv);
    }
}

void dima_init_eventually_failing(struct dima_eventually_failing *dima,
                                  struct dima *next,
                                  size_t n_allocs) {
    unsigned flags = dima_forward_flags(next);
    flags &= ~DIMA_EXITS_ON_FAILURE;
    flags &= ~DIMA_IS_THREAD_SAFE;
    dima_init_proxy(&dima->proxy, invoke_eventually_failing, flags);
    dima->next = next;
    dima->n_allocs = n_allocs;
}
