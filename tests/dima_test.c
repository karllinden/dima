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

#include <dima/dima.h>

#include "test.h"

enum fake_function {
    FAKE_FREE,
    FAKE_ALLOC,
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

static struct fake fake;

static char fake_return[3];

static void fake_free(struct dima *dima, void *ptr) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_FREE;
    fake->ptr = ptr;
}

static void *fake_alloc(struct dima *dima, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_ALLOC;
    fake->size = size;
    return fake_return;
}

static void *fake_realloc(struct dima *dima, void *ptr, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_REALLOC;
    fake->ptr = ptr;
    fake->size = size;
    return fake_return;
}

static void *fake_alloc_array(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_ALLOC_ARRAY;
    fake->nmemb = nmemb;
    fake->size = size;
    return fake_return;
}

static void *fake_alloc_array0(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_ALLOC_ARRAY0;
    fake->nmemb = nmemb;
    fake->size = size;
    return fake_return;
}

static void *fake_realloc_array(struct dima *dima,
                                void *ptr,
                                size_t nmemb,
                                size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_REALLOC_ARRAY;
    fake->ptr = ptr;
    fake->nmemb = nmemb;
    fake->size = size;
    return fake_return;
}

static char *fake_strdup(struct dima *dima, const char *s) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_STRDUP;
    fake->s = s;
    return fake_return;
}

static char *fake_strndup(struct dima *dima, const char *s, size_t n) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_STRNDUP;
    fake->s = s;
    fake->n = n;
    return fake_return;
}

static const struct dima_vtable fake_vtable = {
        fake_free,
        fake_alloc,
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

START_TEST(test_calls_free_fn) {
    int x;
    dima_free(test_dima, &x);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_FREE, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
}
END_TEST

START_TEST(test_calls_alloc_fn) {
    void *ret = dima_alloc(test_dima, 79);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_ALLOC, fake.func);
    ck_assert_uint_eq(79, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_realloc_fn) {
    int x;
    void *ret = dima_realloc(test_dima, &x, 1462);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_REALLOC, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
    ck_assert_uint_eq(1462, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_alloc_array_fn) {
    void *ret = dima_alloc_array(test_dima, 148, 962);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_ALLOC_ARRAY, fake.func);
    ck_assert_uint_eq(148, fake.nmemb);
    ck_assert_uint_eq(962, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_alloc_array0_fn) {
    void *ret = dima_alloc_array0(test_dima, 789, 654);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_ALLOC_ARRAY0, fake.func);
    ck_assert_uint_eq(789, fake.nmemb);
    ck_assert_uint_eq(654, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_realloc_array_fn) {
    int x;
    void *ret = dima_realloc_array(test_dima, &x, 975, 135);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_REALLOC_ARRAY, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
    ck_assert_uint_eq(975, fake.nmemb);
    ck_assert_uint_eq(135, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_strdup_fn) {
    const char *s = "Hello";
    char *ret = dima_strdup(test_dima, s);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_STRDUP, fake.func);
    ck_assert_ptr_eq(s, fake.s);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_strndup_fn) {
    const char *s = "Goodbye";
    char *ret = dima_strndup(test_dima, s, 4);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_STRNDUP, fake.func);
    ck_assert_ptr_eq(s, fake.s);
    ck_assert_uint_eq(4, fake.n);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

void init_test_dima(void) {
    fake_init(&fake);
    test_dima = &fake.dima;
}

void add_tests(Suite *suite) {
    ADD_TEST(calls_free_fn);
    ADD_TEST(calls_alloc_fn);
    ADD_TEST(calls_realloc_fn);
    ADD_TEST(calls_alloc_array_fn);
    ADD_TEST(calls_alloc_array0_fn);
    ADD_TEST(calls_realloc_array_fn);
    ADD_TEST(calls_strdup_fn);
    ADD_TEST(calls_strndup_fn);
}
