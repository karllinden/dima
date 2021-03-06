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

#include <stdlib.h>
#include <string.h>

#include <dima/derived.h>
#include <dima/system.h>

static void system_free(struct dima *dima UNUSED, void *ptr) {
    free(ptr);
}

static void *system_malloc(struct dima *dima UNUSED, size_t size) {
    return malloc(size);
}

static void *system_calloc(struct dima *dima UNUSED,
                           size_t nmemb,
                           size_t size) {
    return calloc(nmemb, size);
}

static void *system_realloc(struct dima *dima UNUSED, void *ptr, size_t size) {
    return realloc(ptr, size);
}

#if HAVE_REALLOCARRAY
static void *system_reallocarray(struct dima *dima UNUSED,
                                 void *ptr,
                                 size_t nmemb,
                                 size_t size) {
    return reallocarray(ptr, nmemb, size);
}
#else
#define system_reallocarray dima_realloc_array_with_realloc
#endif

#if HAVE_STRDUP
static char *system_strdup(struct dima *dima UNUSED, const char *s) {
    return strdup(s);
}
#else
#define system_strdup dima_strdup_with_alloc
#endif

#if HAVE_STRNDUP
static char *system_strndup(struct dima *dima UNUSED, const char *s, size_t n) {
    return strndup(s, n);
}
#else
#define system_strndup dima_strndup_with_alloc
#endif

static const struct dima_vtable vtable = {
        system_free,
        system_malloc,
        dima_alloc0_with_alloc,
        system_realloc,
        dima_alloc_array_with_alloc,
        system_calloc,
        system_reallocarray,
        system_strdup,
        system_strndup,
};

void dima_init_system(struct dima *dima) {
    dima->vtable = &vtable;
    dima->flags = DIMA_IS_THREAD_SAFE;
}
