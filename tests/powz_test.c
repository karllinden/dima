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

/*
 * Tests for the powz functions.
 *
 * Although these functions are just used for testing overflow scenarios, it is
 * very important that they work correctly.
 */

#include <stdlib.h>

#include "powz.h"
#include "test.h"

START_TEST(test_pow3z_0_is_1) {
    ck_assert_uint_eq(1, pow3z(0));
}
END_TEST

static void test_pow3z(size_t n) {
    ck_assert_uint_eq(pow3z(n) * 3, pow3z(n + 1));
}

START_TEST(test_pow3z_works_for_small_n) {
    test_pow3z(_i);
}
END_TEST

START_TEST(test_pow3z_works_for_small_random_n) {
    test_pow3z(rand());
}
END_TEST

START_TEST(test_pow3z_inv_0_is_1) {
    ck_assert_uint_eq(1, pow3z_inv(0));
}
END_TEST

static void test_pow3z_inv(size_t n) {
    ck_assert_uint_eq(1, pow3z(n) * pow3z_inv(n));
}

START_TEST(test_pow3z_inv_works_for_small_n) {
    test_pow3z_inv(_i);
}
END_TEST

START_TEST(test_pow3z_inv_works_for_small_random_n) {
    test_pow3z_inv(rand());
}
END_TEST

void init_test_dima(void) {
    /* Not used by the test cases. */
    test_dima = NULL;
}

void add_tests(Suite *suite) {
    ADD_TEST(pow3z_0_is_1);
    ADD_LOOP_TEST(pow3z_works_for_small_n, 0, 100);
    ADD_LOOP_TEST(pow3z_works_for_small_random_n, 0, 100);
    ADD_TEST(pow3z_inv_0_is_1);
    ADD_LOOP_TEST(pow3z_inv_works_for_small_n, 0, 100);
    ADD_LOOP_TEST(pow3z_inv_works_for_small_random_n, 0, 100);
}
