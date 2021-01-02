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

#include <dima/dima.h>

void *dima_malloc(struct dima *dima, size_t size) {
    return dima->vtable->malloc_fn(dima, size);
}

void dima_free(struct dima *dima, void *ptr) {
    dima->vtable->free_fn(dima, ptr);
}

void *dima_calloc(struct dima *dima, size_t nmemb, size_t size) {
    return dima->vtable->calloc_fn(dima, nmemb, size);
}

void *dima_realloc(struct dima *dima, void *ptr, size_t size) {
    return dima->vtable->realloc_fn(dima, ptr, size);
}

void *dima_mallocarray(struct dima *dima, size_t nmemb, size_t size) {
    return dima->vtable->mallocarray_fn(dima, nmemb, size);
}

void *dima_reallocarray(struct dima *dima,
                        void *ptr,
                        size_t nmemb,
                        size_t size) {
    return dima->vtable->reallocarray_fn(dima, ptr, nmemb, size);
}

char *dima_strdup(struct dima *dima, const char *s) {
    return dima->vtable->strdup_fn(dima, s);
}

char *dima_strndup(struct dima *dima, const char *s, size_t n) {
    return dima->vtable->strndup_fn(dima, s, n);
}
