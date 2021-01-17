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

#include <dima/exiting_on_failure.h>

#include "forwarding_tests.h"
#include "test.h"

static struct dima_exiting_on_failure instance;

START_TEST(test_exits_on_failure) {
    ck_assert_int_ne(0, dima_exits_on_failure(test_dima));
}
END_TEST

void init_test_dima(void) {
    init_forwardee();
    dima_init_exiting_on_failure(&instance, dima_from_fake(&forwardee), 77);
    test_dima = dima_from_exiting_on_failure(&instance);
}

void add_tests(Suite *suite) {
    ADD_TEST(exits_on_failure);
    add_forwarding_tests(suite);
}
