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
 *  + dima_free()
 *  + dima_alloc()
 *  + dima_alloc0()
 *  + dima_realloc()
 *  + dima_alloc_array()
 *  + dima_alloc_array0()
 *  + dima_realloc_array()
 *  + dima_strdup()
 *  + dima_strndup().
 * The signatures of these functions are described in this header. They all take
 * the struct dima * implementation as their first argument, which is not
 * allowed to be NULL.
 *
 * On error the functions in this header return NULL or exit, depending on the
 * implementation. Implementations are required to set the DIMA_EXITS_ON_FAILURE
 * flag in dima->flags if and only if the implementation exits on failure. The
 * flag can be queried with dima_exits_on_failure(). See
 * dima_init_exiting_on_failure() for an implementation that exits on failure.
 *
 * The functions in this header are not required to set errno on failure. This
 * can be achieved by using dima_init_with_failure_hook().
 *
 * The dima_alloc(), dima_realloc() and dima_alloc_array0() behave as malloc(),
 * realloc() and calloc() from the C11 standard library, except that the memory
 * is allocated according to the DIMA implementation and that they may exit on
 * failure.
 *
 * The dima_free() function behaves as the C11 standard library free(), except
 * that it free's memory according to the DIMA implementation.
 *
 * dima_alloc_array() behaves as dima_alloc_array0() except that the returned
 * memory is not initialized to zero.
 *
 * The dima_realloc_array(), dima_strdup() and dima_strndup() functions behave
 * as their glibc counterparts reallocarray(), strdup() and strndup(), except
 * that they allocate memory according to the DIMA implementation, are not
 * required to set errno on failure, and may exit on failure. Unlike in glibc,
 * dima_realloc_array(d, p, 0, 0) is not required to be equivalent to
 * dima_free(d, p).
 *
 * The dima_alloc0() function behaves as dima_alloc() except that the returned
 * memory is initialized to zero.
 *
 * The dima_*alloc_array*() functions fail if the total size, nmemb * size,
 * overflows.
 *
 * Implementations are free to make further guarantees that are not required by
 * this abstraction. For example an implementation may guarantees such as:
 *  + the functions are multi-thread safe
 *  + the functions never exit on failure
 *  + the functions always exit on failure
 *  + errno is always set on error
 *  + dima_alloc(d, 0) always returns non-NULL
 *  + dima_realloc(d, p, 0) behaves as dima_free(d, p)
 *  + etc.
 * Implementors should document any additional guarantees. See for example
 * dima_init_exiting_on_failure and dima_init_mutex_locked. Clients may depend
 * on any additional guarantees, but they should document any additional
 * requirements.
 *
 * For examples of some implementations of this abstraction can see:
 *  + dima/system.h
 *  + dima/env.h (TODO)
 *  + dima/derived.h
 *  + dima/exiting_on_failure.h
 *  + dima/failure_hook.h
 *  + dima/mutex_locked.h (TODO)
 *  + dima/spin_locked.h (TODO)
 *  + dima/thread_local.h (TODO)
 *  + dima/notifying.h (TODO)
 *  + dima/failing.h
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
 * dima_init_derived_vtable and then set any extra functions. See
 * dima/derived.h.
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

/**
 * Flag that should be set in dima->flags if and only if the implementation
 * exits on failure.
 */
#define DIMA_EXITS_ON_FAILURE (1u << 0)

struct dima;

typedef void dima_free_fn(struct dima *dima, void *ptr);

typedef void *dima_alloc_fn(struct dima *dima, size_t size);

typedef void *dima_realloc_fn(struct dima *dima, void *ptr, size_t size);

typedef void *dima_alloc_array_fn(struct dima *dima, size_t nmemb, size_t size);

typedef void *dima_realloc_array_fn(struct dima *dima,
                                    void *ptr,
                                    size_t nmemb,
                                    size_t size);

typedef char *dima_strdup_fn(struct dima *dima, const char *s);

typedef char *dima_strndup_fn(struct dima *dima, const char *s, size_t n);

struct dima_vtable {
    dima_free_fn *free_fn;
    dima_alloc_fn *alloc_fn;
    dima_alloc_fn *alloc0_fn;
    dima_realloc_fn *realloc_fn;
    dima_alloc_array_fn *alloc_array_fn;
    dima_alloc_array_fn *alloc_array0_fn;
    dima_realloc_array_fn *realloc_array_fn;
    dima_strdup_fn *strdup_fn;
    dima_strndup_fn *strndup_fn;
};

struct dima {
    const struct dima_vtable *vtable;
    unsigned flags;
};

void dima_free(struct dima *dima, void *ptr);
void *dima_alloc(struct dima *dima, size_t size);
void *dima_alloc0(struct dima *dima, size_t size);
void *dima_realloc(struct dima *dima, void *ptr, size_t size);
void *dima_alloc_array(struct dima *dima, size_t nmemb, size_t size);
void *dima_alloc_array0(struct dima *dima, size_t nmemb, size_t size);
void *dima_realloc_array(struct dima *dima,
                         void *ptr,
                         size_t nmemb,
                         size_t size);
char *dima_strdup(struct dima *dima, const char *s);
char *dima_strndup(struct dima *dima, const char *s, size_t n);

/**
 * Returns non-zero if and only if the implementation exits on failure.
 *
 * DIMA implementations must declare whether or not they exit on failure by
 * setting DIMA_EXITS_ON_FAILURE in dima->flags appropriately.
 *
 * Prefer this function over checking dima->flags directly.
 */
static inline int dima_exits_on_failure(const struct dima *dima) {
    return dima->flags & DIMA_EXITS_ON_FAILURE;
}

#endif /* !DIMA_DIMA_H */
