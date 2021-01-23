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
#include <dima/proxy/eventually_failing.h>

#include "forwarding_tests.h"
#include "test.h"

static struct dima_eventually_failing instance;

static void make_failing_after(unsigned n_allocs) {
    dima_init_eventually_failing(
            &instance, dima_from_fake(&forwardee), n_allocs);
}

void init_test_dima(void) {
    init_forwardee();
    make_failing_after(1);
    test_dima = dima_from_eventually_failing(&instance);
}

START_TEST(test_does_not_exit_on_failure) {
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_does_not_exit_on_failure_even_if_next_does) {
    struct dima_exiting_on_failure deof;
    dima_init_exiting_on_failure(&deof, dima_from_fake(&forwardee), 28);
    dima_init_eventually_failing(
            &instance, dima_from_exiting_on_failure(&deof), 1);
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_alloc_fails_after_0) {
    make_failing_after(0);
    void *ptr = dima_alloc(test_dima, 8);
    ck_assert_ptr_eq(NULL, ptr);
}
END_TEST

START_TEST(test_alloc0_fails_after_1) {
    make_failing_after(1);
    void *ptr1 = dima_alloc0(test_dima, 16);
    void *ptr2 = dima_alloc0(test_dima, 16);
    ck_assert_ptr_ne(NULL, ptr1);
    ck_assert_ptr_eq(NULL, ptr2);
    dima_free(test_dima, ptr1);
}
END_TEST

START_TEST(test_free_does_not_count) {
    make_failing_after(2);
    void *ptr1 = dima_alloc_array(test_dima, 4, 32);
    ck_assert_ptr_ne(NULL, ptr1);
    dima_free(test_dima, ptr1);

    void *ptr2 = dima_alloc_array0(test_dima, 2, 16);
    ck_assert_ptr_ne(NULL, ptr2);
}
END_TEST

START_TEST(test_second_realloc_fails) {
    make_failing_after(2);
    void *ptr1 = dima_alloc_array(test_dima, 4, 32);
    ck_assert_ptr_ne(NULL, ptr1);

    void *ptr2 = dima_realloc_array(test_dima, ptr1, 8, 32);
    ck_assert_ptr_ne(NULL, ptr2);

    void *ptr3 = dima_realloc_array(test_dima, ptr2, 16, 32);
    ck_assert_ptr_eq(NULL, ptr3);
    dima_free(test_dima, ptr2);
}
END_TEST

void add_tests(Suite *suite) {
    ADD_TEST(does_not_exit_on_failure);
    ADD_TEST(does_not_exit_on_failure_even_if_next_does);
    ADD_TEST(alloc_fails_after_0);
    ADD_TEST(alloc0_fails_after_1);
    ADD_TEST(free_does_not_count);
    ADD_TEST(second_realloc_fails);
    add_forwarding_tests(suite);
}
