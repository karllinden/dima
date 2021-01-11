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

/* Re-usable tests that assert that the test_dima forwards to the forwardee. */

#ifndef FORWARDING_TESTS_H
#define FORWARDING_TESTS_H

#include <check.h>

#include <dima/dima.h>

enum fake_function {
    FAKE_FREE,
    FAKE_ALLOC,
    FAKE_ALLOC0,
    FAKE_REALLOC,
    FAKE_ALLOC_ARRAY,
    FAKE_ALLOC_ARRAY0,
    FAKE_REALLOC_ARRAY,
    FAKE_STRDUP,
    FAKE_STRNDUP,
};

struct fake {
    struct dima dima;
    int count;
    enum fake_function func;
    void *ptr;
    size_t nmemb;
    size_t size;
    const char *s;
    size_t n;
};

struct fake forwardee;

void init_forwardee(void);
void add_forwarding_tests(Suite *suite);

#endif /* !FORWARDING_TESTS_H */