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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dima/proxy/invocation.h>

static void COLD NORETURN fatal_impl(const char *file,
                                     int line,
                                     const char *fmt,
                                     ...) {
    fprintf(stderr, "%s:%d: ", file, line);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fputc('\n', stderr);
    fflush(stderr);
    abort();
}
#define fatal(...) fatal_impl(__FILE__, __LINE__, __VA_ARGS__)

static inline void init_invocation(struct dima_invocation *invocation) {
    /* Because memcmpy is used for dima_compare_invocations any padding bits
     * need to be zero-initialized. */
    memset(invocation, 0, sizeof(*invocation));
}

void dima_init_free_invocation(struct dima_invocation *invocation, void *ptr) {
    init_invocation(invocation);
    invocation->function = DIMA_FREE;
    invocation->ptr = ptr;
}

void dima_init_alloc_invocation(struct dima_invocation *invocation,
                                size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_ALLOC;
    invocation->size = size;
}

void dima_init_alloc0_invocation(struct dima_invocation *invocation,
                                 size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_ALLOC0;
    invocation->size = size;
}

void dima_init_realloc_invocation(struct dima_invocation *invocation,
                                  void *ptr,
                                  size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_REALLOC;
    invocation->ptr = ptr;
    invocation->size = size;
}

void dima_init_alloc_array_invocation(struct dima_invocation *invocation,
                                      size_t nmemb,
                                      size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_ALLOC_ARRAY;
    invocation->nmemb = nmemb;
    invocation->size = size;
}

void dima_init_alloc_array0_invocation(struct dima_invocation *invocation,
                                       size_t nmemb,
                                       size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_ALLOC_ARRAY0;
    invocation->nmemb = nmemb;
    invocation->size = size;
}

void dima_init_realloc_array_invocation(struct dima_invocation *invocation,
                                        void *ptr,
                                        size_t nmemb,
                                        size_t size) {
    init_invocation(invocation);
    invocation->function = DIMA_REALLOC_ARRAY;
    invocation->ptr = ptr;
    invocation->nmemb = nmemb;
    invocation->size = size;
}

void dima_init_strdup_invocation(struct dima_invocation *invocation,
                                 const char *s) {
    init_invocation(invocation);
    invocation->function = DIMA_STRDUP;
    invocation->s = s;
}

void dima_init_strndup_invocation(struct dima_invocation *invocation,
                                  const char *s,
                                  size_t n) {
    init_invocation(invocation);
    invocation->function = DIMA_STRNDUP;
    invocation->s = s;
    invocation->n = n;
}

void *dima_invoke(struct dima *dima, const struct dima_invocation *inv) {
    switch (inv->function) {
        case DIMA_FREE:
            dima_free(dima, inv->ptr);
            return NULL;
        case DIMA_ALLOC:
            return dima_alloc(dima, inv->size);
        case DIMA_ALLOC0:
            return dima_alloc0(dima, inv->size);
        case DIMA_REALLOC:
            return dima_realloc(dima, inv->ptr, inv->size);
        case DIMA_ALLOC_ARRAY:
            return dima_alloc_array(dima, inv->nmemb, inv->size);
        case DIMA_ALLOC_ARRAY0:
            return dima_alloc_array0(dima, inv->nmemb, inv->size);
        case DIMA_REALLOC_ARRAY:
            return dima_realloc_array(dima, inv->ptr, inv->nmemb, inv->size);
        case DIMA_STRDUP:
            return dima_strdup(dima, inv->s);
        case DIMA_STRNDUP:
            return dima_strndup(dima, inv->s, inv->n);
    }

    fatal("Invalid function %d in invocation.", inv->function);
}

int dima_compare_invocations(const struct dima_invocation *a,
                             const struct dima_invocation *b) {
    return memcmp(a, b, sizeof(*a));
}
