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

#include <dima/failing.h>

static void failing_free() {
    /* Cannot fail. */
}

static void *failing_alloc(struct dima *dima UNUSED, size_t size UNUSED) {
    return NULL;
}

static void *failing_realloc(struct dima *dima UNUSED,
                             void *ptr UNUSED,
                             size_t size UNUSED) {
    return NULL;
}

static void *failing_alloc_array(struct dima *dima UNUSED,
                                 size_t nmemb UNUSED,
                                 size_t size UNUSED) {
    return NULL;
}

static void *failing_realloc_array(struct dima *dima UNUSED,
                                   void *ptr UNUSED,
                                   size_t nmemb UNUSED,
                                   size_t size UNUSED) {
    return NULL;
}

static char *failing_strdup(struct dima *dima UNUSED, const char *s UNUSED) {
    return NULL;
}

static char *failing_strndup(struct dima *dima UNUSED,
                             const char *s UNUSED,
                             size_t size UNUSED) {
    return NULL;
}

static const struct dima_vtable failing_vtable = {
        failing_free,
        failing_alloc,
        failing_alloc,
        failing_realloc,
        failing_alloc_array,
        failing_alloc_array,
        failing_realloc_array,
        failing_strdup,
        failing_strndup,
};

void dima_init_failing(struct dima *dima) {
    dima->vtable = &failing_vtable;
}
