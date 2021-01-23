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

#include <dima/failure_hook.h>

static inline struct dima_with_failure_hook *downcast(struct dima *dima) {
    return (struct dima_with_failure_hook *)dima;
}

static void COLD call_failure_hook(const struct dima_with_failure_hook *impl) {
    impl->hook(impl->ptr);
}

static inline void *handle_result(struct dima_with_failure_hook *impl,
                                  void *result) {
    if (result == NULL) {
        call_failure_hook(impl);
    }
    return result;
}

static void failure_hook_free(struct dima *dima, void *ptr) {
    struct dima_with_failure_hook *impl = downcast(dima);
    dima_free(impl->next, ptr);
}

static void *failure_hook_alloc(struct dima *dima, size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_alloc(impl->next, size));
}

static void *failure_hook_alloc0(struct dima *dima, size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_alloc0(impl->next, size));
}

static void *failure_hook_realloc(struct dima *dima, void *ptr, size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_realloc(impl->next, ptr, size));
}

static void *failure_hook_alloc_array(struct dima *dima,
                                      size_t nmemb,
                                      size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_alloc_array(impl->next, nmemb, size));
}

static void *failure_hook_alloc_array0(struct dima *dima,
                                       size_t nmemb,
                                       size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_alloc_array0(impl->next, nmemb, size));
}

static void *failure_hook_realloc_array(struct dima *dima,
                                        void *ptr,
                                        size_t nmemb,
                                        size_t size) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl,
                         dima_realloc_array(impl->next, ptr, nmemb, size));
}

static char *failure_hook_strdup(struct dima *dima, const char *s) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_strdup(impl->next, s));
}

static char *failure_hook_strndup(struct dima *dima, const char *s, size_t n) {
    struct dima_with_failure_hook *impl = downcast(dima);
    return handle_result(impl, dima_strndup(impl->next, s, n));
}

static const struct dima_vtable failure_hook_vtable = {
        failure_hook_free,
        failure_hook_alloc,
        failure_hook_alloc0,
        failure_hook_realloc,
        failure_hook_alloc_array,
        failure_hook_alloc_array0,
        failure_hook_realloc_array,
        failure_hook_strdup,
        failure_hook_strndup,
};

void dima_init_with_failure_hook(struct dima_with_failure_hook *dima,
                                 struct dima *next,
                                 dima_failure_fn *hook,
                                 void *ptr) {
    dima->dima.vtable = &failure_hook_vtable;
    dima->dima.flags = next->flags & DIMA_EXITS_ON_FAILURE;
    dima->next = next;
    dima->hook = hook;
    dima->ptr = ptr;
}
