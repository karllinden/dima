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

#include <assert.h>
#include <string.h>

#include <dima/pthread_mutex_locked.h>

static void *invoke_pthread_mutex_locked(
        struct dima_proxy *proxy,
        const struct dima_invocation *invocation) {
    struct dima_pthread_mutex_locked *impl
            = (struct dima_pthread_mutex_locked *)proxy;
    pthread_mutex_lock(&impl->mutex);
    void *result = dima_invoke(impl->next, invocation);
    pthread_mutex_unlock(&impl->mutex);
    return result;
}

int dima_init_pthread_mutex_locked(struct dima_pthread_mutex_locked *dima,
                                   struct dima *next) {
    assert(!dima_is_thread_hostile(next));

    unsigned flags = dima_forward_flags(next);
    flags |= DIMA_IS_THREAD_SAFE;
    dima_init_proxy(&dima->proxy, invoke_pthread_mutex_locked, flags);
    int status = pthread_mutex_init(&dima->mutex, NULL);
    dima->next = next;
    return status;
}

void dima_destroy_pthread_mutex_locked(struct dima_pthread_mutex_locked *dima) {
    pthread_mutex_destroy(&dima->mutex);
    memset(dima, 0, sizeof(*dima));
}
