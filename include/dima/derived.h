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
 * dima/derived.h
 * --------------
 * Derived implementations of the DIMA functions.
 *
 * It is possible to implement some of the DIMA functions in terms of others.
 * For example dima_mallocarray can be implemented with dima_malloc. This header
 * declares the derived implementations provided by the library.
 *
 * The naming convention for functions in this header is dima_x_with_y, where
 * dima_x is the function being implemented and dima_y is the function used for
 * the implementation. For example dima_mallocarray_with_malloc implements
 * dima_mallocarray by using dima_malloc.
 */

#ifndef DIMA_DERIVED_H
#define DIMA_DERIVED_H

#include <dima/dima.h>

void *dima_mallocarray_with_malloc(struct dima *dima,
                                   size_t nmemb,
                                   size_t size);

void *dima_reallocarray_with_realloc(struct dima *dima,
                                     void *ptr,
                                     size_t nmemb,
                                     size_t size);

char *dima_strdup_with_malloc(struct dima *dima, const char *s);

char *dima_strndup_with_malloc(struct dima *dima, const char *s, size_t n);

#endif /* !DIMA_DERIVED_H */
