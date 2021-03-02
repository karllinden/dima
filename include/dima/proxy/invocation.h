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

/**
 * dima/proxy/invocation.h
 * -----------------------
 * Functions that allow invocations of DIMA functions to be treated as data.
 *
 * These functions are useful for testing and for creating proxies. To
 * initialize an invocation, use one of the dima_init_*_invocation functions.
 * After an invocation has been initialized it can be invoked upon a dima, with
 * the dima_invoke function.
 */

#ifndef DIMA_PROXY_INVOCATION_H
#define DIMA_PROXY_INVOCATION_H

#include <dima/dima.h>

enum dima_function {
    DIMA_FREE,

    DIMA_ALLOC,
    DIMA_ALLOC0,
    DIMA_REALLOC,
    DIMA_ALLOC_ARRAY,
    DIMA_ALLOC_ARRAY0,
    DIMA_REALLOC_ARRAY,

    DIMA_STRDUP,
    DIMA_STRNDUP,
};

struct dima_invocation {
    enum dima_function function;
    union {
        /* DIMA_FREE, DIMA_ALLOC, DIMA_ALLOC0, DIMA_REALLOC, DIMA_ALLOC_ARRAY,
         * DIMA_ALLOC_ARRAY0, DIMA_REALLOC_ARRAY */
        struct {
            void *ptr;
            size_t nmemb;
            size_t size;
        };

        /* DIMA_STRDUP, DIMA_STRNDUP */
        struct {
            const char *s;
            size_t n;
        };
    };
};

void dima_init_free_invocation(struct dima_invocation *invocation, void *ptr);
void dima_init_alloc_invocation(struct dima_invocation *invocation,
                                size_t size);
void dima_init_alloc0_invocation(struct dima_invocation *invocation,
                                 size_t size);
void dima_init_realloc_invocation(struct dima_invocation *invocation,
                                  void *ptr,
                                  size_t size);
void dima_init_alloc_array_invocation(struct dima_invocation *invocation,
                                      size_t nmemb,
                                      size_t size);
void dima_init_alloc_array0_invocation(struct dima_invocation *invocation,
                                       size_t nmemb,
                                       size_t size);
void dima_init_realloc_array_invocation(struct dima_invocation *invocation,
                                        void *ptr,
                                        size_t nmemb,
                                        size_t size);
void dima_init_strdup_invocation(struct dima_invocation *invocation,
                                 const char *s);
void dima_init_strndup_invocation(struct dima_invocation *invocation,
                                  const char *s,
                                  size_t n);

/**
 * Invokes the invocation on the DIMA.
 *
 * For example, this is equivalent to calling dima_alloc_array0(dima, 5, 16):
 *
 * struct dima_invocation inv;
 * dima_init_alloc_array0_invocation(&inv, 5, 16);
 * void *ret = dima_invoke(dima, &inv);
 *
 * Returns the result of the invocation, or NULL if the function to invoke is
 * dima_free().
 */
void *dima_invoke(struct dima *dima, const struct dima_invocation *inv);

/**
 * Compares two invocations in an unspecified (but fixed) total order.
 *
 * Returns an integer less than, equal to or greater than zero, if a is found,
 * respectively, to be less than, equal to or greater than b.
 */
int dima_compare_invocations(const struct dima_invocation *a,
                             const struct dima_invocation *b);

/**
 * Copies the invocation at src to the invocation at dest.
 */
void dima_copy_invocation(struct dima_invocation *dest,
                          const struct dima_invocation *src);

#endif /* !DIMA_PROXY_INVOCATION_H */
