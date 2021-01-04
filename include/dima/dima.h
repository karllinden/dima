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
 * Declaration of the dependency-injectible memory allocator (DIMA) abtraction.
 *
 * The DIMA abstraction supports the following functions:
 *  + dima_malloc()
 *  + dima_free()
 *  + dima_calloc()
 *  + dima_realloc()
 *  + dima_mallocarray()
 *  + dima_reallocarray()
 *  + dima_strdup()
 *  + dima_strndup().
 * The signatures of these functions are described in this header. They all take
 * the struct dima * implementation as their first argument, which is not
 * allowed to be NULL. On error these functions return NULL or exit, depending
 * on the implementation. (See for example dima_init_exiting_on_failure().) On
 * error these functions are not required to set the errno variable.
 *
 * The dima_malloc(), dima_calloc(), dima_realloc() behave as their C11 standard
 * library counterparts, except that the memory is allocated according to the
 * DIMA implementation and that they may exit on failure.
 *
 * The dima_free() function behaves as the C11 standard library free(), except
 * that it free's memory according to the DIMA implementation.
 *
 * dima_mallocarray() behaves as dima_calloc() except that the returned memory
 * is not initialized to zero.
 *
 * The dima_reallocarray(), dima_strdup() and dima_strndup() functions behave as
 * their glibc counterparts, except that they allocate memory according to the
 * DIMA implementation, are not required to set errno on failure, and may exit
 * on failure. Unlike in glibc, dima_reallocarray(d, p, 0, 0) is not required to
 * be equivalent to dima_free(d, p).
 *
 * The dima_calloc(), dima_mallocarray() and dima_reallocarray() fail if the
 * total size, nmemb * size, overflows.
 *
 * Implementations are free to make further guarantees that are not required by
 * this abstraction. For example an implementation may guarantees such as:
 *  + the functions are multi-thread safe
 *  + the functions never exit on failure
 *  + the functions always exit on failure
 *  + errno is always set on error
 *  + dima_malloc(d, 0) always returns non-NULL
 *  + dima_realloc(d, p, 0) behaves as dima_free(d, p)
 *  + etc.
 * Implementors should document any additional guarantees. See for example
 * dima_init_exiting_on_failure and dima_init_mutex_locked. Clients may depend
 * on any additional guarantees, but they should document any additional
 * requirements.
 *
 * For examples of some implementations of this abstraction can see:
 *  + dima/system.h (TODO)
 *  + dima/malloc_and_free.h (TODO)
 *  + dima/realloc_and_free.h (TODO)
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
 * with a custom dima_vtable containing the functions that should be called on
 * the structure. Any data necessary for the implementation must be placed after
 * the the composed dima structure. All function pointers in the vtable must be
 * set to non-NULL values, and the pointed to functions must behave as described
 * above. To avoid re-implementing some functions, implementors can use
 * dima_init_vtable_with_malloc_and_free (TODO) or
 * dima_init_vtable_with_realloc_and_free (TODO) and then set any extra
 * functions.
 *
 * Implementations should document whether or not they are safe for
 * multi-threaded use, because that is not required by this abstraction. As long
 * as an implementation is not thread hostile, it can be made thread safe by
 * wrapping in a dima_mutex_locked (TODO) or dima_spin_locked (TODO). See
 * dima/mutex_locked.h (TODO) and dima/spin_locked.h (TODO) respectively for
 * more information.
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
