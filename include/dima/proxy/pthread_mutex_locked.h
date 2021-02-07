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
 * dima/proxy/pthread_mutex_locked.h
 * -------------------------
 * A DIMA proxy that forwards to an other implementation while synchronizing on
 * a pthread_mutex_t.
 *
 * If the next implementation is not thread hostile, then this implementation is
 * thread-safe. If the next implementation is thread-hostile, behvaior is
 * undefined.
 *
 * Including this header introduces a compile-time dependency on pthread.h. If
 * the platform does not support pthread.h, then the functions declared in this
 * header are not included in built library.
 */

#ifndef DIMA_PROXY_PTHREAD_MUTEX_LOCKED_H
#define DIMA_PROXY_PTHREAD_MUTEX_LOCKED_H

#include <pthread.h>

#include <dima/proxy/proxy.h>

struct dima_pthread_mutex_locked {
    struct dima_proxy proxy;
    pthread_mutex_t mutex;
    struct dima *next;
};

/**
 * Initializes the given DIMA to forward to the next implementation while
 * synchronizing on a pthread_mutex_t.
 *
 * The initialized DIMA is thread-safe. If the next DIMA is thread-hostile, the
 * behavior is undefined.
 *
 * A successfully initialized struct dima_pthread_mutex_locked must be destroyed
 * with dima_destroy_pthread_mutex_locked when it is no longer used.
 *
 * @param dima the pthread_mutex_t locked dima
 * @param next the next DIMA implementation
 * @return the result of pthread_mutex_init, i.e. zero on success and a status
 *         code on failure
 */
int dima_init_pthread_mutex_locked(struct dima_pthread_mutex_locked *dima,
                                   struct dima *next);

void dima_destroy_pthread_mutex_locked(struct dima_pthread_mutex_locked *dima);

static inline struct dima *dima_from_pthread_mutex_locked(
        struct dima_pthread_mutex_locked *dima) {
    return dima_from_proxy(&dima->proxy);
}

#endif /* !DIMA_PROXY_PTHREAD_MUTEX_LOCKED_H */
