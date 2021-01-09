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

#include <dima/derived.h>

#include "size_bits.h"

#define SQRT_SIZE_MAX (((size_t)1) << (SIZE_BITS / 2))

static inline int array_size_overflows(size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    return (nmemb >= SQRT_SIZE_MAX || size >= SQRT_SIZE_MAX)
           && (bytes / nmemb != size);
}

void *dima_mallocarray_with_malloc(struct dima *dima,
                                   size_t nmemb,
                                   size_t size) {
    if (array_size_overflows(nmemb, size)) {
        return NULL;
    }
    return dima_malloc(dima, nmemb * size);
}