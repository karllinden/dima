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

#include <dima/system.h>

#include "standard_tests.h"
#include "test.h"

struct dima instance;

START_TEST(test_system_is_thread_safe) {
    ck_assert_int_ne(0, dima_is_thread_safe(&instance));
}
END_TEST

void init_test_dima(void) {
    dima_init_system(&instance);
    test_dima = &instance;
}

void add_tests(Suite *suite) {
    ADD_TEST(system_is_thread_safe);
    add_standard_tests(suite);
}
