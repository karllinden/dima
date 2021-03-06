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
#include <string.h>

#include "test.h"

struct dima *test_dima = NULL;

static const char *determine_suite_name(const char *arg) {
    const char *s = strrchr(arg, '/');
    return s != NULL ? s + 1 : arg;
}

static void teardown(void) {
    /* Nothing to do. */
}

static TCase *create_test(Suite *suite, const char *name) {
    TCase *tc = tcase_create(name);
    tcase_add_checked_fixture(tc, init_test_dima, teardown);
    suite_add_tcase(suite, tc);
    return tc;
}

void add_test(Suite *suite, const char *name, TFun func) {
    TCase *tc = create_test(suite, name);
    tcase_add_test(tc, func);
}

void add_loop_test(Suite *suite,
                   const char *name,
                   TFun func,
                   int start,
                   int end) {
    TCase *tc = create_test(suite, name);
    tcase_add_loop_test(tc, func, start, end);
}

int main(int argc __attribute__((unused)), const char *argv[]) {
    Suite *suite = suite_create(determine_suite_name(argv[0]));
    add_tests(suite);

    SRunner *srunner = srunner_create(suite);
    srunner_run_all(srunner, CK_NORMAL);
    int n_failed = srunner_ntests_failed(srunner);
    srunner_free(srunner);

    return n_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
