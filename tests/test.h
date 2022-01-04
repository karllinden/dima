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

#ifndef TEST_H
#define TEST_H

#include <check.h>

#include <dima/dima.h>

#define ADD_TEST(name) add_test(suite, #name, test_##name)
#define ADD_LOOP_TEST(name, start, end) \
    add_loop_test(suite, #name, test_##name, start, end)

extern struct dima *test_dima;

/* Provided by each of the test programs. */
void init_test_dima(void);
void add_tests(Suite *suite);

/* Provided by test.c. */
void add_test(Suite *suite, const char *name, TFun func);
void add_loop_test(Suite *suite,
                   const char *name,
                   TFun func,
                   int start,
                   int end);

#endif /* !TEST_H */
