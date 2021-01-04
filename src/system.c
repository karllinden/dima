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

#include <dima/system.h>

static void *system_malloc(struct dima *dima __attribute__((unused)),
                           size_t size) {
    return malloc(size);
}

static void system_free(struct dima *dima __attribute__((unused)), void *ptr) {
    free(ptr);
}

static const struct dima_vtable vtable = {
        system_malloc,
        system_free,
        NULL, /* TODO */
        NULL, /* TODO */
        NULL, /* TODO */
        NULL, /* TODO */
        NULL, /* TODO */
        NULL, /* TODO */
};

void dima_init_system(struct dima *dima) {
    dima->vtable = &vtable;
}
