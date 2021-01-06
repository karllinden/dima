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

#include "standard_tests.h"
#include "powz.h"
#include "test.h"

/* Wrapper around dima_malloc(test_dima, size) or dima_realloc(test_dima, NULL,
 * size). */
typedef void *alloc_fn(size_t size);

/* Wrapper around dima_calloc, dima_reallocarray or dima_mallocarray. */
typedef void *allocarray_fn(size_t nmemb, size_t size);

static void test_returns_null_on_failure(alloc_fn *alloc) {
    /* Valgrind reports that the input is suspicious if a division is not used
     * here, but allocating 5/4 * SIZE_MAX should fail even if it is done in
     * five allocations. */
    void *ptrs[5];
    for (int i = 0; i < 5; ++i) {
        ptrs[i] = alloc(SIZE_MAX / 4);
    }
    ck_assert(ptrs[0] == NULL //
              || ptrs[1] == NULL //
              || ptrs[2] == NULL //
              || ptrs[3] == NULL //
              || ptrs[4] == NULL);
    for (int i = 0; i < 5; ++i) {
        dima_free(test_dima, ptrs[i]);
    }
}

static void test_returns_null_on_overflow(allocarray_fn *allocarray, int i) {
    int small_nmemb = i % 2;
    i /= 2;

    size_t weight = ((size_t)1 << (i % 4));
    i /= 4;

    /* +1 because pow3z(0) = 1 and pow3z_inv(0) = 1, which does not produce an
     * overflow. */
    size_t n = i + 1;

    size_t small = weight * pow3z(n);
    size_t large = pow3z_inv(n);
    size_t nmemb = small_nmemb ? small : large;
    size_t size = small_nmemb ? large : small;

    void *ptr = allocarray(nmemb, size);
    ck_assert_msg(ptr == NULL,
                  "Expected array allocation to fail with:\n"
                  "    nmemb = %zu * %zu = %zu\n"
                  "    size = %zu\n",
                  nmemb,
                  size);
}

START_TEST(test_malloc_0_works) {
    void *ptr = dima_malloc(test_dima, 0);
    /* ptr may be NULL or non-NULL, but at least it should not crash. */
    dima_free(test_dima, ptr);
}
END_TEST

START_TEST(test_malloc_1_returns_non_null) {
    void *ptr = dima_malloc(test_dima, 1);
    ck_assert_ptr_ne(NULL, ptr);
    dima_free(test_dima, ptr);
}
END_TEST

START_TEST(test_small_malloc_returns_non_null) {
    void *ptr = dima_malloc(test_dima, 64);
    ck_assert_ptr_ne(NULL, ptr);
    dima_free(test_dima, ptr);
}
END_TEST

START_TEST(test_malloced_memory_is_writable) {
    void *ptr = dima_malloc(test_dima, 98);
    memset(ptr, 0, 98);
    dima_free(test_dima, ptr);
}
END_TEST

static void *test_dima_malloc(size_t size) {
    return dima_malloc(test_dima, size);
}

START_TEST(test_malloc_returns_null_on_failure) {
    test_returns_null_on_failure(test_dima_malloc);
}
END_TEST

START_TEST(test_free_null_works) {
    dima_free(test_dima, NULL);
}
END_TEST

START_TEST(test_calloc_0_0_works) {
    void *ptr = dima_calloc(test_dima, 0, 0);
    /* ptr may be NULL or non-NULL, but at least it should not crash. */
    dima_free(test_dima, ptr);
}
END_TEST

START_TEST(test_calloc_1_1_works) {
    void *ptr = dima_calloc(test_dima, 1, 1);
    ck_assert_ptr_ne(NULL, ptr);
    dima_free(test_dima, ptr);
}
END_TEST

START_TEST(test_calloced_memory_is_zeroed) {
    int *ints = dima_calloc(test_dima, 16, sizeof(int));
    for (int i = 0; i < 16; ++i) {
        ck_assert_int_eq(0, ints[i]);
    }
    dima_free(test_dima, ints);
}
END_TEST

static void *test_dima_calloc(size_t nmemb, size_t size) {
    return dima_calloc(test_dima, nmemb, size);
}

START_TEST(test_calloc_checks_for_overflow) {
    test_returns_null_on_overflow(test_dima_calloc, _i);
}
END_TEST

void add_standard_tests(Suite *suite) {
    /* malloc */
    ADD_TEST(malloc_0_works);
    ADD_TEST(malloc_1_returns_non_null);
    ADD_TEST(small_malloc_returns_non_null);
    ADD_TEST(malloced_memory_is_writable);
    ADD_TEST(malloc_returns_null_on_failure);

    /* free */
    ADD_TEST(free_null_works);

    /* calloc */
    ADD_TEST(calloc_0_0_works);
    ADD_TEST(calloc_1_1_works);
    ADD_TEST(calloced_memory_is_zeroed);
    ADD_LOOP_TEST(calloc_checks_for_overflow, 0, 256);
}
