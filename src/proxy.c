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

#include <dima/proxy.h>

static inline void *invoke_proxy(struct dima *dima,
                                 const struct dima_invocation *invocation) {
    return dima_invoke_proxy((struct dima_proxy *)dima, invocation);
}

static void proxy_free(struct dima *dima, void *ptr) {
    struct dima_invocation inv;
    dima_init_free_invocation(&inv, ptr);
    invoke_proxy(dima, &inv);
}

static void *proxy_alloc(struct dima *dima, size_t size) {
    struct dima_invocation inv;
    dima_init_alloc_invocation(&inv, size);
    return invoke_proxy(dima, &inv);
}

static void *proxy_alloc0(struct dima *dima, size_t size) {
    struct dima_invocation inv;
    dima_init_alloc0_invocation(&inv, size);
    return invoke_proxy(dima, &inv);
}

static void *proxy_realloc(struct dima *dima, void *ptr, size_t size) {
    struct dima_invocation inv;
    dima_init_realloc_invocation(&inv, ptr, size);
    return invoke_proxy(dima, &inv);
}

static void *proxy_alloc_array(struct dima *dima, size_t nmemb, size_t size) {
    struct dima_invocation inv;
    dima_init_alloc_array_invocation(&inv, nmemb, size);
    return invoke_proxy(dima, &inv);
}

static void *proxy_alloc_array0(struct dima *dima, size_t nmemb, size_t size) {
    struct dima_invocation inv;
    dima_init_alloc_array0_invocation(&inv, nmemb, size);
    return invoke_proxy(dima, &inv);
}

static void *proxy_realloc_array(struct dima *dima,
                                 void *ptr,
                                 size_t nmemb,
                                 size_t size) {
    struct dima_invocation inv;
    dima_init_realloc_array_invocation(&inv, ptr, nmemb, size);
    return invoke_proxy(dima, &inv);
}

static char *proxy_strdup(struct dima *dima, const char *s) {
    struct dima_invocation inv;
    dima_init_strdup_invocation(&inv, s);
    return invoke_proxy(dima, &inv);
}

static char *proxy_strndup(struct dima *dima, const char *s, size_t n) {
    struct dima_invocation inv;
    dima_init_strndup_invocation(&inv, s, n);
    return invoke_proxy(dima, &inv);
}

static const struct dima_vtable proxy_vtable = {
        proxy_free,
        proxy_alloc,
        proxy_alloc0,
        proxy_realloc,
        proxy_alloc_array,
        proxy_alloc_array0,
        proxy_realloc_array,
        proxy_strdup,
        proxy_strndup,
};

void dima_init_proxy(struct dima_proxy *proxy,
                     dima_invoke_proxy_fn *invoke_proxy_fn,
                     unsigned flags) {
    proxy->dima.vtable = &proxy_vtable;
    proxy->dima.flags = flags | DIMA_IS_PROXY;
    proxy->invoke_proxy_fn = invoke_proxy_fn;
}

void *dima_invoke_proxy(struct dima_proxy *proxy,
                        const struct dima_invocation *invocation) {
    return proxy->invoke_proxy_fn(proxy, invocation);
}
