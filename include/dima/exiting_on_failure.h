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

/**
 * dima/exiting_on_failure.h
 * -------------------------
 * A DIMA implementation that wraps another DIMA and exits with a given status
 * on failure.
 *
 * The implementation is thread safe if the implementation passed as next
 * parameter to dima_init_exiting_on_failure() is.
 */

#ifndef DIMA_EXITING_ON_FAILURE_H
#define DIMA_EXITING_ON_FAILURE_H

#include <dima/failure_hook.h>

struct dima_exiting_on_failure {
    struct dima_with_failure_hook with_hook;
    int status;
};

/**
 * Initializes the given dima to forward to the next DIMA implementation and
 * exit with the given status on failure.
 */
void dima_init_exiting_on_failure(struct dima_exiting_on_failure *dima,
                                  struct dima *next,
                                  int status);

static inline struct dima *dima_from_exiting_on_failure(
        struct dima_exiting_on_failure *dima) {
    return dima_from_with_failure_hook(&dima->with_hook);
}

#endif /* !DIMA_EXITING_ON_FAILURE_H */
