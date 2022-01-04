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

/*
 * dima/derived.h
 * --------------
 * Derived implementations of the DIMA functions.
 *
 * It is possible to implement some of the DIMA functions in terms of others.
 * For example dima_alloc_array can be implemented with dima_alloc. This header
 * declares the derived implementations provided by the library.
 *
 * The naming convention for functions in this header is dima_x_with_y, where
 * dima_x is the function being implemented and dima_y is the function used for
 * the implementation. For example dima_alloc_array_with_alloc implements
 * dima_alloc_array by using dima_alloc.
 *
 * The implementations do not depend on the concrete struct dima implementation,
 * so clients can use any implementation of DIMA with the derived functions
 * declared in this header.
 *
 * To create a complete implementation derived from implementations of free()
 * and realloc(), use dima_init_derived_vtable().
 */

#ifndef DIMA_DERIVED_H
#define DIMA_DERIVED_H

#include <dima/dima.h>

void *dima_alloc_with_realloc(struct dima *dima, size_t size);

void *dima_alloc0_with_alloc(struct dima *dima, size_t size);

void *dima_alloc_array_with_alloc(struct dima *dima, size_t nmemb, size_t size);

void *dima_alloc_array0_with_alloc_array(struct dima *dima,
                                         size_t nmemb,
                                         size_t size);

void *dima_realloc_array_with_realloc(struct dima *dima,
                                      void *ptr,
                                      size_t nmemb,
                                      size_t size);

char *dima_strdup_with_alloc(struct dima *dima, const char *s);

char *dima_strndup_with_alloc(struct dima *dima, const char *s, size_t n);

/**
 * Initializes the given vtable with a complete implementation derived from the
 * given free and realloc functions.
 *
 * The derived functions do not depend on which type of DIMA is used.
 *
 * @param vtable the vtable to initialize
 * @param free_fn the free() function
 * @param realloc_fn the realloc() function
 */
void dima_init_derived_vtable(struct dima_vtable *vtable,
                              dima_free_fn *free_fn,
                              dima_realloc_fn *realloc_fn);

#endif /* !DIMA_DERIVED_H */
