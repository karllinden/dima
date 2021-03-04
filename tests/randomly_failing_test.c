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
#include <time.h>

#include <dima/exiting_on_failure.h>
#include <dima/randomly_failing.h>
#include <dima/system.h>

#include "forwarding_tests.h"
#include "test.h"

static struct dima_randomly_failing instance;

START_TEST(test_does_not_exit_on_failure) {
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_does_not_exit_on_failure_even_if_next_does) {
    struct dima_exiting_on_failure deof;
    dima_init_exiting_on_failure(&deof, dima_from_fake(&forwardee), 28);
    dima_init_randomly_failing(
            &instance, dima_from_exiting_on_failure(&deof), 1);
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_is_not_thread_safe) {
    struct dima system;
    dima_init_system(&system);
    dima_init_randomly_failing(&instance, &system, 0);
    ck_assert_int_eq(0, dima_is_thread_safe(test_dima));
}
END_TEST

START_TEST(test_is_thread_hostile) {
    ck_assert_int_ne(0, dima_is_thread_hostile(test_dima));
}
END_TEST

static int count_failures(void) {
    int result = 0;
    for (int i = 0; i < 100; ++i) {
        void *ptr = dima_alloc(test_dima, 16);
        result += (ptr == NULL);
        dima_free(test_dima, ptr);
    }
    return result;
}

START_TEST(test_never_fails_if_failure_percentage_is_0) {
    int failure_count = count_failures();
    ck_assert_int_eq(0, failure_count);
}
END_TEST

START_TEST(test_always_fails_if_failure_percentage_is_100) {
    dima_init_randomly_failing(&instance, dima_from_fake(&forwardee), 100);
    int failure_count = count_failures();
    ck_assert_int_eq(100, failure_count);
}
END_TEST

START_TEST(test_has_ok_failure_count_if_failure_percentage_is_50) {
    dima_init_randomly_failing(&instance, dima_from_fake(&forwardee), 50);
    int failure_count = count_failures();

    /* The probability for failure_count <= 7 || failure_count >= 93 is
     * approximately 2.72 * 10^-20, and that probability is so vanishingly
     * small that we deem it impossible. */
    ck_assert_int_gt(failure_count, 7);
    ck_assert_int_lt(failure_count, 93);
}
END_TEST

void init_test_dima(void) {
    init_forwardee();
    dima_init_randomly_failing(&instance, dima_from_fake(&forwardee), 0);
    test_dima = dima_from_randomly_failing(&instance);
}

void add_tests(Suite *suite) {
    srand(time(NULL));

    ADD_TEST(does_not_exit_on_failure);
    ADD_TEST(does_not_exit_on_failure_even_if_next_does);
    ADD_TEST(is_not_thread_safe);
    ADD_TEST(is_thread_hostile);
    ADD_TEST(never_fails_if_failure_percentage_is_0);
    ADD_TEST(always_fails_if_failure_percentage_is_100);
    ADD_TEST(has_ok_failure_count_if_failure_percentage_is_50);
    add_forwarding_tests(suite);
}
