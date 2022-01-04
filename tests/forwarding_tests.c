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

#include "forwarding_tests.h"

#include "invocations.h"
#include "test.h"

struct fake forwardee;

static char fake_return[3];

static void *invoke_fake(struct dima_proxy *proxy,
                         const struct dima_invocation *invocation) {
    struct fake *fake = (struct fake *)proxy;
    fake->count++;
    dima_copy_invocation(&fake->invocation, invocation);
    return fake_return;
}

static void fake_init(struct fake *fake) {
    dima_init_proxy(&fake->proxy, invoke_fake, 0);
    fake->count = 0;
}

START_TEST(test_calls_function) {
    const struct dima_invocation *inv = invocations + _i;
    void *ret = dima_invoke(test_dima, inv);
    ck_assert_int_eq(1, forwardee.count);
    ck_assert_int_eq(0, dima_compare_invocations(&forwardee.invocation, inv));
    if (inv->function != DIMA_FREE) {
        ck_assert_ptr_eq(fake_return, ret);
    }
}
END_TEST

void init_forwardee(void) {
    fake_init(&forwardee);
}

void add_forwarding_tests(Suite *suite) {
    ADD_LOOP_TEST(calls_function, 0, N_INVOCATIONS);
}
