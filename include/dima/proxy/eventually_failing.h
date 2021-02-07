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

/*
 * dima/proxy/eventually_failing.h
 * -------------------------
 * A DIMA proxy that fails after a given number of allocations.
 *
 * This implementation is not thread-safe, but is not thread hostile, so it can
 * be made thread safe by adding synchronization.
 */

#ifndef DIMA_PROXY_EVENTUALLY_FAILING_H
#define DIMA_PROXY_EVENTUALLY_FAILING_H

#include <dima/proxy/proxy.h>

struct dima_eventually_failing {
    struct dima_proxy proxy;
    struct dima *next;
    size_t n_allocs;
};

/**
 * Initializes the given dima to forward to the next implementation and fail
 * after the given number of allocations.
 */
void dima_init_eventually_failing(struct dima_eventually_failing *dima,
                                  struct dima *next,
                                  size_t n_allocs);

static inline struct dima *dima_from_eventually_failing(
        struct dima_eventually_failing *dima) {
    return dima_from_proxy(&dima->proxy);
}

#endif /* !DIMA_PROXY_EVENTUALLY_FAILING_H */
