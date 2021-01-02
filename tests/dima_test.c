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
    FAKE_MALLOC,
    FAKE_FREE,
    FAKE_CALLOC,
    FAKE_REALLOC,
    FAKE_MALLOCARRAY,
    FAKE_REALLOCARRAY,
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

static char fake_return[3];

static void *fake_malloc(struct dima *dima, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_MALLOC;
    fake->size = size;
    return fake_return;
}

static void fake_free(struct dima *dima, void *ptr) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_FREE;
    fake->ptr = ptr;
}

static void *fake_calloc(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_CALLOC;
    fake->nmemb = nmemb;
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

static void *fake_mallocarray(struct dima *dima, size_t nmemb, size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_MALLOCARRAY;
    fake->nmemb = nmemb;
    fake->size = size;
    return fake_return;
}

static void *fake_reallocarray(struct dima *dima,
                               void *ptr,
                               size_t nmemb,
                               size_t size) {
    struct fake *fake = (struct fake *)dima;
    fake->count++;
    fake->func = FAKE_REALLOCARRAY;
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
        fake_malloc,
        fake_free,
        fake_calloc,
        fake_realloc,
        fake_mallocarray,
        fake_reallocarray,
        fake_strdup,
        fake_strndup,
};

static void fake_init(struct fake *fake) {
    fake->dima.vtable = &fake_vtable;
    fake->count = 0;
}

START_TEST(test_calls_malloc_fn) {
    struct fake fake;
    fake_init(&fake);

    void *ret = dima_malloc(&fake.dima, 79);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_MALLOC, fake.func);
    ck_assert_uint_eq(79, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_free_fn) {
    struct fake fake;
    fake_init(&fake);

    int x;
    dima_free(&fake.dima, &x);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_FREE, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
}
END_TEST

START_TEST(test_calls_calloc_fn) {
    struct fake fake;
    fake_init(&fake);

    void *ret = dima_calloc(&fake.dima, 789, 654);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_CALLOC, fake.func);
    ck_assert_uint_eq(789, fake.nmemb);
    ck_assert_uint_eq(654, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_realloc_fn) {
    struct fake fake;
    fake_init(&fake);

    int x;
    void *ret = dima_realloc(&fake.dima, &x, 1462);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_REALLOC, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
    ck_assert_uint_eq(1462, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_mallocarray_fn) {
    struct fake fake;
    fake_init(&fake);

    void *ret = dima_mallocarray(&fake.dima, 148, 962);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_MALLOCARRAY, fake.func);
    ck_assert_uint_eq(148, fake.nmemb);
    ck_assert_uint_eq(962, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_reallocarray_fn) {
    struct fake fake;
    fake_init(&fake);

    int x;
    void *ret = dima_reallocarray(&fake.dima, &x, 975, 135);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_REALLOCARRAY, fake.func);
    ck_assert_ptr_eq(&x, fake.ptr);
    ck_assert_uint_eq(975, fake.nmemb);
    ck_assert_uint_eq(135, fake.size);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_strdup_fn) {
    struct fake fake;
    fake_init(&fake);

    const char *s = "Hello";
    char *ret = dima_strdup(&fake.dima, s);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_STRDUP, fake.func);
    ck_assert_ptr_eq(s, fake.s);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

START_TEST(test_calls_strndup_fn) {
    struct fake fake;
    fake_init(&fake);

    const char *s = "Goodbye";
    char *ret = dima_strndup(&fake.dima, s, 4);
    ck_assert_int_eq(1, fake.count);
    ck_assert_int_eq(FAKE_STRNDUP, fake.func);
    ck_assert_ptr_eq(s, fake.s);
    ck_assert_uint_eq(4, fake.n);
    ck_assert_ptr_eq(fake_return, ret);
}
END_TEST

void add_tests(Suite *suite) {
    ADD_TEST(calls_malloc_fn);
    ADD_TEST(calls_free_fn);
    ADD_TEST(calls_calloc_fn);
    ADD_TEST(calls_realloc_fn);
    ADD_TEST(calls_mallocarray_fn);
    ADD_TEST(calls_reallocarray_fn);
    ADD_TEST(calls_strdup_fn);
    ADD_TEST(calls_strndup_fn);
}
