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

#include "forwarding_tests.h"

#include "invocations.h"
#include "test.h"

static char fake_return[3];

static void fake_free(struct dima *dima, void *ptr) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_free_invocation(&fake->invocation, ptr);
}

static void *fake_alloc(struct dima *dima, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_alloc_invocation(&fake->invocation, size);
    return fake_return;
}

static void *fake_alloc0(struct dima *dima, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_alloc0_invocation(&fake->invocation, size);
    return fake_return;
}

static void *fake_realloc(struct dima *dima, void *ptr, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_realloc_invocation(&fake->invocation, ptr, size);
    return fake_return;
}

static void *fake_alloc_array(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_alloc_array_invocation(&fake->invocation, nmemb, size);
    return fake_return;
}

static void *fake_alloc_array0(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_alloc_array0_invocation(&fake->invocation, nmemb, size);
    return fake_return;
}

static void *fake_realloc_array(struct dima *dima,
                                void *ptr,
                                size_t nmemb,
                                size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_realloc_array_invocation(&fake->invocation, ptr, nmemb, size);
    return fake_return;
}

static char *fake_strdup(struct dima *dima, const char *s) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_strdup_invocation(&fake->invocation, s);
    return fake_return;
}

static char *fake_strndup(struct dima *dima, const char *s, size_t n) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    dima_init_strndup_invocation(&fake->invocation, s, n);
    return fake_return;
}

static const struct dima_vtable fake_vtable = {
        fake_free,
        fake_alloc,
        fake_alloc0,
        fake_realloc,
        fake_alloc_array,
        fake_alloc_array0,
        fake_realloc_array,
        fake_strdup,
        fake_strndup,
};

static void fake_init(struct fake *fake) {
    fake->dima.vtable = &fake_vtable;
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
