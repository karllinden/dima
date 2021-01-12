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
#include <dima/failing.h>
#include <dima/failure_hook.h>
#include <dima/system.h>

#include "forwarding_tests.h"
#include "test.h"

typedef void *function_under_test_fn(void);

static struct dima failing;
static struct dima_with_failure_hook instance;

/* We only care about the address. */
static char my_data;

static int my_hook_count;

static void my_hook(void *ptr) {
    ck_assert_ptr_eq(ptr, &my_data);
    my_hook_count++;
}

static void make_failing(void) {
    dima_init_failing(&failing);
    dima_init_with_failure_hook(&instance, &failing, my_hook, &my_data);
}

void init_test_dima(void) {
    my_hook_count = 0;
    init_forwardee();
    dima_init_with_failure_hook(&instance, &forwardee.dima, my_hook, &my_data);
    test_dima = dima_from_with_failure_hook(&instance);
}

static void *test_alloc(void) {
    return dima_alloc(test_dima, 8);
}

static void *test_alloc0(void) {
    return dima_alloc0(test_dima, 40);
}

static void *test_realloc(void) {
    return dima_realloc(test_dima, NULL, 20);
}

static void *test_alloc_array(void) {
    return dima_alloc_array(test_dima, 23, 25);
}

static void *test_alloc_array0(void) {
    return dima_alloc_array0(test_dima, 12, 21);
}

static void *test_realloc_array(void) {
    return dima_realloc_array(test_dima, NULL, 89, 50);
}

static void *test_strdup(void) {
    return dima_strdup(test_dima, "Hello!");
}

static void *test_strndup(void) {
    return dima_strndup(test_dima, "Goodbye!", 6);
}

static function_under_test_fn *functions[] = {
        test_alloc,
        test_alloc0,
        test_realloc,
        test_alloc_array,
        test_alloc_array0,
        test_realloc_array,
        test_strdup,
        test_strndup,
};

START_TEST(test_exits_on_failure_if_next_does) {
    struct dima_exiting_on_failure deof;
    dima_init_exiting_on_failure(&deof, &failing, 77);
    dima_init_with_failure_hook(
            &instance, dima_from_exiting_on_failure(&deof), my_hook, &my_data);
    ck_assert_int_ne(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_does_not_exit_on_failure_if_next_does_not) {
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_calls_hook_on_failure) {
    function_under_test_fn *function = functions[_i];
    make_failing();
    void *ret = function();
    ck_assert_ptr_eq(NULL, ret);
    ck_assert_int_eq(1, my_hook_count);
}
END_TEST

void add_tests(Suite *suite) {
    add_forwarding_tests(suite);
    int n_functions = sizeof(functions) / sizeof(*functions);
    ADD_TEST(exits_on_failure_if_next_does);
    ADD_TEST(does_not_exit_on_failure_if_next_does_not);
    ADD_LOOP_TEST(calls_hook_on_failure, 0, n_functions);
}
