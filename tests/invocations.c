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

#include "invocations.h"

static char dummy;

struct dima_invocation invocations[N_INVOCATIONS];

void init_invocations(void) {
    dima_init_free_invocation(invocations + 0, &dummy);
    dima_init_alloc_invocation(invocations + 1, 8);
    dima_init_alloc0_invocation(invocations + 2, 40);
    dima_init_realloc_invocation(invocations + 3, &dummy, 20);
    dima_init_alloc_array_invocation(invocations + 4, 23, 25);
    dima_init_alloc_array0_invocation(invocations + 5, 12, 21);
    dima_init_realloc_array_invocation(invocations + 6, &dummy, 89, 50);
    dima_init_strdup_invocation(invocations + 7, "Hello!");
    dima_init_strndup_invocation(invocations + 8, "Goodbye!", 6);
}
