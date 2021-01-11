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
 * dima/failure_hook.h
 * -------------------
 * A DIMA implementation that invokes a failure function (a hook) on failure.
 *
 * The implementation is thread safe if the implementation passed as next
 * parameter to dima_init_with_failure_hook() is.
 */

#ifndef DIMA_FAILURE_HOOK_H
#define DIMA_FAILURE_HOOK_H

#include <dima/dima.h>

typedef void dima_failure_fn(void *ptr);

struct dima_with_failure_hook {
    struct dima dima;
    struct dima *next;
    dima_failure_fn *hook;
    void *ptr;
};

/**
 * Initializes the given dima to forward to the next DIMA implementation and
 * calling the hook with the pointer on failure.
 */
void dima_init_with_failure_hook(struct dima_with_failure_hook *dima,
                                 struct dima *next,
                                 dima_failure_fn *hook,
                                 void *ptr);

static inline struct dima *dima_from_with_failure_hook(
        struct dima_with_failure_hook *dima) {
    return &dima->dima;
}

#endif /* !DIMA_FAILURE_HOOK_H */
