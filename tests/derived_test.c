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

#include <stdlib.h>

#include <dima/derived.h>

#include "standard_tests.h"
#include "test.h"

static struct dima_vtable vtable;
static struct dima instance;

static void my_free(struct dima *dima UNUSED, void *ptr) {
    free(ptr);
}

static void *my_realloc(struct dima *dima UNUSED, void *ptr, size_t size) {
    return realloc(ptr, size);
}

void init_test_dima(void) {
    dima_init_derived_vtable(&vtable, my_free, my_realloc);
    instance.vtable = &vtable;
    test_dima = &instance;
}

void add_tests(Suite *suite) {
    add_standard_tests(suite);
}
