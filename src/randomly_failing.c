/*
 * Copyright (C) 2021-2022 Karl Linden <karl.j.linden@gmail.com>
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

#include <assert.h>
#include <stdlib.h>

#include <dima/randomly_failing.h>

#define LIMIT (RAND_MAX - RAND_MAX % 100)

/* Returns a random integer in the range [0,100). */
static int rand100(void) {
    int r;
    do {
        r = rand();
    } while (r >= LIMIT);
    return r % 100;
}

static void *invoke_randomly_failing(struct dima_proxy *proxy,
                                     const struct dima_invocation *invocation) {
    struct dima_randomly_failing *impl = (struct dima_randomly_failing *)proxy;
    int r = rand100();
    if (r >= impl->failure_percentage) {
        return dima_invoke(impl->next, invocation);
    } else {
        return NULL;
    }
}

void dima_init_randomly_failing(struct dima_randomly_failing *dima,
                                struct dima *next,
                                int failure_percentage) {
    assert(failure_percentage >= 0 && failure_percentage <= 100);

    unsigned flags = dima_forward_flags(next);
    flags &= ~DIMA_EXITS_ON_FAILURE;
    flags &= ~DIMA_IS_THREAD_SAFE;
    flags |= DIMA_IS_THREAD_HOSTILE;

    dima_init_proxy(&dima->proxy, invoke_randomly_failing, flags);
    dima->next = next;
    dima->failure_percentage = failure_percentage;
}
