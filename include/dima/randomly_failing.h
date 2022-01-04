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
 * dima/randomly_failing.h
 * -----------------------
 * A DIMA proxy that fails randomly with a given rate.
 *
 * For portability, this implementation is based on rand(3) and is therefore
 * thread hostile. If you need a thread-safe implementation, or an
 * implementation with higher quality randomness, consider writing a custom
 * dima_proxy implementation.
 *
 * Before using this implementation you should seed the random number generator
 * with srand(3).
 */

#ifndef DIMA_RANDOMLY_FAILING_H
#define DIMA_RANDOMLY_FAILING_H

#include <dima/proxy.h>

struct dima_randomly_failing {
    struct dima_proxy proxy;
    struct dima *next;
    int failure_percentage;
};

/**
 * Initializes the given dima to forward to the next DIMA implementation and
 * fail randomly with the given percentage.
 *
 * A failure_percentage of 0 causes the initialized dima to never fail.
 * On the other extreme, a failure rate of 100 causes the dima to always fail.
 * Calling this function with a failure_percentage that is less than 0 or
 * greater than 100 invokes undefined behavior.
 *
 * @param dima the dima to initialize
 * @param next the implementation to forward to on success
 * @param failure_percentage the failure rate in percent
 */
void dima_init_randomly_failing(struct dima_randomly_failing *dima,
                                struct dima *next,
                                int failure_percentage);

static inline struct dima *dima_from_randomly_failing(
        struct dima_randomly_failing *dima) {
    return dima_from_proxy(&dima->proxy);
}

#endif /* !DIMA_RANDOMLY_FAILING_H */
