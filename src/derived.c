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

#include <string.h>

#include <dima/derived.h>

#include "size_bits.h"

#define SQRT_SIZE_MAX (((size_t)1) << (SIZE_BITS / 2))

static inline int array_size_overflows(size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    return (nmemb >= SQRT_SIZE_MAX || size >= SQRT_SIZE_MAX) //
           && nmemb > 0 //
           && (bytes / nmemb != size);
}

static inline size_t min_size(size_t a, size_t b) {
    return a < b ? a : b;
}

void *dima_mallocarray_with_malloc(struct dima *dima,
                                   size_t nmemb,
                                   size_t size) {
    if (array_size_overflows(nmemb, size)) {
        return NULL;
    }
    return dima_malloc(dima, nmemb * size);
}

void *dima_reallocarray_with_realloc(struct dima *dima,
                                     void *ptr,
                                     size_t nmemb,
                                     size_t size) {
    if (array_size_overflows(nmemb, size)) {
        return NULL;
    }
    return dima_realloc(dima, ptr, nmemb * size);
}

char *dima_strdup_with_malloc(struct dima *dima, const char *s) {
    size_t size = strlen(s) + 1;
    char *dup = dima_malloc(dima, size);
    if (dup != NULL) {
        memcpy(dup, s, size);
    }
    return dup;
}

char *dima_strndup_with_malloc(struct dima *dima, const char *s, size_t n) {
    size_t len = min_size(strlen(s), n);
    char *dup = dima_malloc(dima, len + 1);
    if (dup != NULL) {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }
    return dup;
}
