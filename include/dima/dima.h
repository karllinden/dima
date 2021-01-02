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
 * dima/dima.h
 * -----------
 * Declarations of the dependency-injectible memory allocator (DIMA) abtraction.
 *
 * Implementations of the DIMA abstraction can be found in the other
 * headers, such as:
 *  + dima/system.h (TODO)
 *  + dima/malloc_and_free.h (TODO)
 *  + dima/realloc.h (TODO)
 *  + dima/exiting_on_failure.h (TODO)
 *  + dima/mutex_locked.h (TODO)
 *  + dima/spin_locked.h (TODO)
 *  + dima/thread_local.h (TODO)
 *  + dima/notifying.h (TODO)
 *  + dima/failing.h (TODO)
 *  + dima/eventually_failing.h (TODO)
 *  + dima/randomly_failing.h (TODO)
 *
 * Clients outside the DIMA library may implement this abstraction, by creating
 * a struct that has a struct dima as first member and filling the composed dima
 * with a custom dima_vtable with the functions that should be called on the
 * structure. Any data necessary for the implementation can be placed after the
 * the composed dima structure.  All function pointers in the vtable must be set
 * to non-NULL values. To avoid re-implementing some functions, implementors can
 * use dima_init_vtable_with_malloc_and_free (TODO) or
 * dima_init_vtable_with_realloc (TODO) and then set any extra functions.
 *
 * Implementations should document whether or not they are safe for
 * multi-threaded use, because that is not required by this abstraction. As long
 * as an implementation is not thread hostile, it can be made thread safe by
 * wrapping in a dima_mutex_locked (TODO) or dima_spin_locked (TODO). See the
 * corresponding headers for more information.
 *
 * The functions behave as their standard library counterparts, except that they
 * take a struct dima. For example dima_malloc allocates a memory block of a
 * given size. However, note that implementations may exit on failure (instead
 * of returning NULL). This is what dima_exiting_on_error (TODO) does. The dima
 * used for dima_free must match the dima used for the allocating function, such
 * as dima_malloc or dima_strdup.
 */

#ifndef DIMA_DIMA_H
#define DIMA_DIMA_H

#include <stddef.h>

struct dima;

typedef void *dima_malloc_fn(struct dima *dima, size_t size);

typedef void dima_free_fn(struct dima *dima, void *ptr);

typedef void *dima_calloc_fn(struct dima *dima, size_t nmemb, size_t size);

typedef void *dima_realloc_fn(struct dima *dima, void *ptr, size_t size);

typedef void *dima_mallocarray_fn(struct dima *dima, size_t nmemb, size_t size);

typedef void *dima_reallocarray_fn(struct dima *dima,
                                   void *ptr,
                                   size_t nmemb,
                                   size_t size);

typedef char *dima_strdup_fn(struct dima *dima, const char *s);

typedef char *dima_strndup_fn(struct dima *dima, const char *s, size_t n);

struct dima_vtable {
    dima_malloc_fn *malloc_fn;
    dima_free_fn *free_fn;
    dima_calloc_fn *calloc_fn;
    dima_realloc_fn *realloc_fn;
    dima_mallocarray_fn *mallocarray_fn;
    dima_reallocarray_fn *reallocarray_fn;
    dima_strdup_fn *strdup_fn;
    dima_strndup_fn *strndup_fn;
};

struct dima {
    const struct dima_vtable *vtable;
};

void *dima_malloc(struct dima *dima, size_t size);

void dima_free(struct dima *dima, void *ptr);

void *dima_calloc(struct dima *dima, size_t nmemb, size_t size);

void *dima_realloc(struct dima *dima, void *ptr, size_t size);

void *dima_mallocarray(struct dima *dima, size_t nmemb, size_t size);

void *dima_reallocarray(struct dima *dima,
                        void *ptr,
                        size_t nmemb,
                        size_t size);

char *dima_strdup(struct dima *dima, const char *s);

char *dima_strndup(struct dima *dima, const char *s, size_t n);

#endif /* !DIMA_DIMA_H */
