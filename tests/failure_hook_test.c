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

#include <dima/exiting_on_failure.h>
#include <dima/failing.h>
#include <dima/failure_hook.h>
#include <dima/invocation.h>
#include <dima/system.h>

#include "forwarding_tests.h"
#include "invocations.h"
#include "test.h"

static struct dima_proxy failing;
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
    dima_init_with_failure_hook(
            &instance, dima_from_proxy(&failing), my_hook, &my_data);
}

void init_test_dima(void) {
    my_hook_count = 0;
    init_forwardee();
    dima_init_with_failure_hook(
            &instance, dima_from_fake(&forwardee), my_hook, &my_data);
    test_dima = dima_from_with_failure_hook(&instance);
}

START_TEST(test_exits_on_failure_if_next_does) {
    struct dima_exiting_on_failure deof;
    dima_init_exiting_on_failure(&deof, dima_from_proxy(&failing), 77);
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
    const struct dima_invocation *invocation = invocations + _i;
    if (invocation->function == DIMA_FREE) {
        /* dima_free() never fails. */
        return;
    }

    make_failing();
    void *ret = dima_invoke(test_dima, invocation);
    ck_assert_ptr_eq(NULL, ret);
    ck_assert_int_eq(1, my_hook_count);
}
END_TEST

void add_tests(Suite *suite) {
    add_forwarding_tests(suite);
    ADD_TEST(exits_on_failure_if_next_does);
    ADD_TEST(does_not_exit_on_failure_if_next_does_not);

    init_invocations();
    ADD_LOOP_TEST(calls_hook_on_failure, 0, N_INVOCATIONS);
}
