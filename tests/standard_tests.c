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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <dima/invocation.h>

#include "powz.h"
#include "standard_tests.h"
#include "test.h"

/* Flag for the dima_*alloc_array* functions. */
#define ARRAY 0x1

/* Flag for the dima_realloc* functions. */
#define REALLOC 0x2

/* Flag for dima_alloc0 and dima_alloc_array0. */
#define ZERO 0x4

/* Exclusive maximum value for the loop end of a test. */
#define MAX_LOOP_END 512

struct test_data;

/* The type of the test functions. */
typedef void test_fn(struct test_data *data);

/* A structure adapts a function so that tests can be generated. */
struct function {
    const char *name;
    enum dima_function fn;
    unsigned flags;
};

/* A structure that represents a test that can be run on a function under test
 * if it has the required_flags. */
struct test {
    const char *name;
    test_fn *fn;
    unsigned required_flags;
    int loop_end;
};

/*
 * An instance of a test, which is a pair of a function and a test to make on
 * that function.
 *
 * This is used to generate a good test name, such as alloc_with_size_0_works,
 * or realloc_with_ptr_null_and_size_0_works.
 */
struct instance {
    int function_index;
    int test_index;
    const struct function *function;
    const struct test *test;
    const char *ptr;
    const char *nmemb;
};

/*
 * The structure to pass to the test function and the function under test.
 *
 * This structure is filled in differently depending on which test is run and
 * what flags the function under test has.
 */
struct test_data {
    struct instance instance;
    struct dima_invocation invocation;
    int param;
};

static void *call_function_under_test(const struct test_data *data);

/*
 * Static array that holds all test names.
 *
 * This is needed because check does not strdup() the test case names.
 */
#define NAME_BUFFER_CAPACITY 4096
static char name_buffer[NAME_BUFFER_CAPACITY];
static size_t name_buffer_size = 0;

/* The tests in this header break if the implementation exits on failure. */
START_TEST(test_does_not_exit_on_failure) {
    ck_assert_int_eq(0, dima_exits_on_failure(test_dima));
}
END_TEST

START_TEST(test_free_null_works) {
    dima_free(test_dima, NULL);
}
END_TEST

START_TEST(test_memory_allocated_with_alloc_array0_is_zeroed) {
    int *ints = dima_alloc_array0(test_dima, 16, sizeof(int));
    for (int i = 0; i < 16; ++i) {
        ck_assert_int_eq(0, ints[i]);
    }
    dima_free(test_dima, ints);
}
END_TEST

START_TEST(test_strdup_empty_works) {
    char *s = dima_strdup(test_dima, "");
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strdup_non_empty_works) {
    char *s = dima_strdup(test_dima, "The quick brown fox...");
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("The quick brown fox...", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strdup_returns_new_pointer) {
    const char *orig = "... jumps over the lazy dog.";
    char *s = dima_strdup(test_dima, orig);
    ck_assert_ptr_ne(orig, s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strdup_returns_writable_memory) {
    char *s = dima_strdup(test_dima, "Foobar");
    s[2] = 'g';
    s[5] = 'z';
    ck_assert_str_eq("Fogbaz", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_empty_0_works) {
    char *s = dima_strndup(test_dima, "", 0);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_empty_1_works) {
    char *s = dima_strndup(test_dima, "", 1);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_empty_2_works) {
    char *s = dima_strndup(test_dima, "", 1);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_with_small_n_works) {
    char *s = dima_strndup(test_dima, "What is love?", 6);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("What i", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_with_exact_n_works) {
    char *s = dima_strndup(test_dima, "Baby don't hurt me.", 19);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("Baby don't hurt me.", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_with_large_n_works) {
    char *s = dima_strndup(test_dima, "Don't hurt me.", 19);
    ck_assert_ptr_ne(NULL, s);
    ck_assert_str_eq("Don't hurt me.", s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_returns_new_pointer) {
    const char *orig = "No more.";
    char *s = dima_strndup(test_dima, orig, 16);
    ck_assert_ptr_ne(orig, s);
    dima_free(test_dima, s);
}
END_TEST

START_TEST(test_strndup_returns_writable_memory) {
    char *s = dima_strndup(test_dima, "Dododo", 4);
    s[0] = 'G';
    s[2] = 'g';
    ck_assert_str_eq("Gogo", s);
    dima_free(test_dima, s);
}
END_TEST

static void test_works_when_size_0(struct test_data *data) {
    data->invocation.size = 0;
    void *ptr = call_function_under_test(data);
    /* ptr may be NULL or non-NULL, but at least it should not crash. */
    dima_free(test_dima, ptr);
}

static void test_returns_non_null_when_size_is_1(struct test_data *data) {
    data->invocation.size = 1;
    void *ptr = call_function_under_test(data);
    ck_assert_ptr_ne(NULL, ptr);
    dima_free(test_dima, ptr);
}

static void test_returns_non_null_when_size_is_small(struct test_data *data) {
    data->invocation.size = 64;
    void *ptr = call_function_under_test(data);
    ck_assert_ptr_ne(NULL, ptr);
    dima_free(test_dima, ptr);
}

static void test_returns_writable_memory(struct test_data *data) {
    data->invocation.size = 98;
    void *ptr = call_function_under_test(data);
    memset(ptr, 'c', 98);
    dima_free(test_dima, ptr);
}

static void test_returns_null_on_failure(struct test_data *data) {
    /* Valgrind reports that the input is suspicious if a division is not used
     * here, but allocating 5/4 * SIZE_MAX should fail even if it is done in
     * five allocations. */
    void *ptrs[5];
    data->invocation.size = SIZE_MAX / 4;
    for (int i = 0; i < 5; ++i) {
        ptrs[i] = call_function_under_test(data);
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

static void test_returns_writable_array(struct test_data *data) {
    data->invocation.nmemb = 112;
    data->invocation.size = 10;
    void *ptr = call_function_under_test(data);
    memset(ptr, '@', data->invocation.nmemb * data->invocation.size);
    dima_free(test_dima, ptr);
}

static void test_works_when_nmemb_is_0(struct test_data *data) {
    data->invocation.nmemb = 0;
    data->invocation.size = SIZE_MAX / 4;
    void *ptr = call_function_under_test(data);
    /* ptr may be NULL or non-NULL, but at least it should not crash. */
    dima_free(test_dima, ptr);
}

static void test_returns_null_on_overflow(struct test_data *data) {
    int small_nmemb = data->param % 2;
    data->param /= 2;

    size_t weight = ((size_t)1 << (data->param % 4));
    data->param /= 4;

    /* +1 because pow3z(0) = 1 and pow3z_inv(0) = 1, which does not produce an
     * overflow. */
    size_t n = data->param + 1;

    size_t small = weight * pow3z(n);
    size_t large = pow3z_inv(n);
    data->invocation.nmemb = small_nmemb ? small : large;
    data->invocation.size = small_nmemb ? large : small;

    void *ptr = call_function_under_test(data);
    ck_assert_msg(ptr == NULL,
                  "Expected array allocation to fail with:\n"
                  "    nmemb = %zu * %zu = %zu\n"
                  "    size = %zu\n",
                  data->invocation.nmemb,
                  data->invocation.size);
}

static size_t min_size(size_t a, size_t b) {
    return a < b ? a : b;
}

#define INITIAL_REALLOC_SIZE 27
static void test_realloc_works(struct test_data *data, size_t new_size) {
    const char *s = "abcdefghijklmnopqrstuvwxyz";
    char *ptr = dima_alloc(test_dima, INITIAL_REALLOC_SIZE);
    memcpy(ptr, s, INITIAL_REALLOC_SIZE);

    data->invocation.ptr = ptr;
    data->invocation.size = new_size;
    char *new_ptr = call_function_under_test(data);

    ck_assert_ptr_ne(NULL, new_ptr);
    ck_assert_int_eq(
            0, memcmp(s, new_ptr, min_size(INITIAL_REALLOC_SIZE, new_size)));

    dima_free(test_dima, new_ptr);
}

static void test_works_when_new_size_is_smaller(struct test_data *data) {
    test_realloc_works(data, INITIAL_REALLOC_SIZE - 16);
}

static void test_works_when_new_size_is_equal(struct test_data *data) {
    test_realloc_works(data, INITIAL_REALLOC_SIZE);
}

static void test_works_when_new_size_is_larger(struct test_data *data) {
    test_realloc_works(data, INITIAL_REALLOC_SIZE + 16);
}

static void test_allocates_additional_writable_memory(struct test_data *data) {
    const char *s = "abcdefghijklmnopqrstuvwxyz";
    char *ptr = dima_alloc(test_dima, INITIAL_REALLOC_SIZE);
    memcpy(ptr, s, INITIAL_REALLOC_SIZE);

    data->invocation.ptr = ptr;
    data->invocation.size = INITIAL_REALLOC_SIZE * 2 - 1;
    char *new_ptr = call_function_under_test(data);

    ck_assert_ptr_ne(NULL, new_ptr);
    memcpy(new_ptr + INITIAL_REALLOC_SIZE - 1, s, INITIAL_REALLOC_SIZE);

    dima_free(test_dima, new_ptr);
}

static void test_returns_zero_initialized_memory(struct test_data *data) {
    data->invocation.size = 27;
    unsigned char *buf = call_function_under_test(data);
    for (int i = 0; i < 27; ++i) {
        ck_assert_uint_eq(0, buf[i]);
    }
    dima_free(test_dima, buf);
}

static const struct function functions[] = {
        {"alloc", DIMA_ALLOC, 0},
        {"alloc0", DIMA_ALLOC0, ZERO},
        {"realloc", DIMA_REALLOC, REALLOC},
        {"alloc_array", DIMA_ALLOC_ARRAY, ARRAY},
        {"alloc_array0", DIMA_ALLOC_ARRAY0, ARRAY | ZERO},
        {"realloc_array", DIMA_REALLOC_ARRAY, REALLOC | ARRAY},
};
#define N_FUNCTIONS (sizeof(functions) / sizeof(functions[0]))

#define TEST(name, flags) \
    { #name, test_##name, flags, 1 }
#define LOOP_TEST(name, flags, loop_end) \
    { #name, test_##name, flags, loop_end }
static const struct test tests[] = {
        TEST(works_when_size_0, 0),
        TEST(returns_non_null_when_size_is_1, 0),
        TEST(returns_non_null_when_size_is_small, 0),
        TEST(returns_writable_memory, 0),
        TEST(returns_null_on_failure, 0),

        TEST(returns_writable_array, ARRAY),
        TEST(works_when_nmemb_is_0, ARRAY),
        LOOP_TEST(returns_null_on_overflow, ARRAY, 256),

        TEST(works_when_new_size_is_smaller, REALLOC),
        TEST(works_when_new_size_is_equal, REALLOC),
        TEST(works_when_new_size_is_larger, REALLOC),
        TEST(allocates_additional_writable_memory, REALLOC),

        TEST(returns_zero_initialized_memory, ZERO),
};
#define N_TESTS (sizeof(tests) / sizeof(tests[0]))

static void add_bytes_to_name(const char *buf, size_t size) {
    if (name_buffer_size + size > NAME_BUFFER_CAPACITY) {
        fputs("No more space in name_buffer.\n", stderr);
        abort();
    }

    memcpy(name_buffer + name_buffer_size, buf, size);
    name_buffer_size += size;
}

static void add_string_to_name(const char *s) {
    /* Omitting '\0' here is intentional. */
    size_t len = strlen(s);
    add_bytes_to_name(s, len);
}

static void add_parameter_to_name(const char *param,
                                  const char *param_name,
                                  const char **with) {
    if (param != NULL) {
        add_string_to_name("_");
        add_string_to_name(*with);
        add_string_to_name("_");
        add_string_to_name(param_name);
        add_string_to_name("_");
        add_string_to_name(param);
        *with = "and";
    }
}

static int should_adapt(const struct instance *inst, unsigned flag) {
    return (inst->function->flags & flag)
           && !(inst->test->required_flags & flag);
}

static void adapt_instance(struct instance *inst) {
    if (should_adapt(inst, REALLOC)) {
        inst->ptr = "null";
    }
    if (should_adapt(inst, ARRAY)) {
        inst->nmemb = "1";
    }
}

static void init_instance(struct instance *inst,
                          int function_index,
                          int test_index) {
    inst->function_index = function_index;
    inst->test_index = test_index;
    inst->function = functions + function_index;
    inst->test = tests + test_index;
    inst->ptr = NULL;
    inst->nmemb = NULL;

    adapt_instance(inst);
}

static int instance_id_from_param(int *param) {
    int id = *param;
    *param %= MAX_LOOP_END;
    id /= MAX_LOOP_END;
    return id;
}

static void init_instance_from_param(struct instance *inst, int *param) {
    int id = instance_id_from_param(param);
    int test_index = id % N_TESTS;
    int function_index = id / N_TESTS;
    init_instance(inst, function_index, test_index);
}

static int is_instance_valid(const struct instance *i) {
    unsigned act_flags = i->function->flags;
    unsigned req_flags = i->test->required_flags;
    return (act_flags & req_flags) == req_flags;
}

static int instance_id(const struct instance *inst) {
    return N_TESTS * inst->function_index + inst->test_index;
}

static int instance_loop_base(const struct instance *inst) {
    return MAX_LOOP_END * instance_id(inst);
}

static const char *instance_name(const struct instance *inst) {
    const char *name = name_buffer + name_buffer_size;

    add_string_to_name(inst->function->name);

    const char *with = "with";
    add_parameter_to_name(inst->ptr, "ptr", &with);
    add_parameter_to_name(inst->nmemb, "nmemb", &with);

    add_string_to_name("_");
    add_string_to_name(inst->test->name);

    add_bytes_to_name("", 1);
    return name;
}

static void init_test_data(struct test_data *d, int i) {
    init_instance_from_param(&d->instance, &i);

    /* We need values for ptr and nmemb that make the realloc and array
     * functions behave as the corresponding functions without realloc and
     * array. Thus, we initialize the invocation as realloc_array and only
     * modify the function code so that the function under test is called. */
    dima_init_realloc_array_invocation(&d->invocation, NULL, 1, 0);
    d->invocation.function = d->instance.function->fn;

    d->param = i;
}

static void *call_function_under_test(const struct test_data *data) {
    return dima_invoke(test_dima, &data->invocation);
}

static void call_test_function(struct test_data *data) {
    data->instance.test->fn(data);
}

START_TEST(dispatch_test) {
    struct test_data d;
    init_test_data(&d, _i);
    call_test_function(&d);
}
END_TEST

static void add_instance(Suite *suite, struct instance *inst) {
    int loop_base = instance_loop_base(inst);
    add_loop_test(suite,
                  instance_name(inst),
                  dispatch_test,
                  loop_base,
                  loop_base + inst->test->loop_end);
}

static void handle_instance(Suite *suite, struct instance *inst) {
    if (is_instance_valid(inst)) {
        add_instance(suite, inst);
    }
}

void add_standard_tests(Suite *suite) {
    ADD_TEST(does_not_exit_on_failure);
    ADD_TEST(free_null_works);
    ADD_TEST(memory_allocated_with_alloc_array0_is_zeroed);
    ADD_TEST(strdup_empty_works);
    ADD_TEST(strdup_non_empty_works);
    ADD_TEST(strdup_returns_new_pointer);
    ADD_TEST(strdup_returns_writable_memory);
    ADD_TEST(strndup_empty_0_works);
    ADD_TEST(strndup_empty_1_works);
    ADD_TEST(strndup_empty_2_works);
    ADD_TEST(strndup_with_small_n_works);
    ADD_TEST(strndup_with_exact_n_works);
    ADD_TEST(strndup_with_large_n_works);
    ADD_TEST(strndup_returns_new_pointer);
    ADD_TEST(strndup_returns_writable_memory);

    for (size_t i = 0; i < N_FUNCTIONS; ++i) {
        for (size_t j = 0; j < N_TESTS; ++j) {
            struct instance inst;
            init_instance(&inst, i, j);
            handle_instance(suite, &inst);
        }
    }
}
