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

#include <stdlib.h>

#include <dima/exiting_on_failure.h>

static void COLD exit_on_failure(void *ptr) {
    struct dima_exiting_on_failure *dima = ptr;
    exit(dima->status);
}

void dima_init_exiting_on_failure(struct dima_exiting_on_failure *dima,
                                  struct dima *next,
                                  int status) {
    dima_init_with_failure_hook(&dima->with_hook, next, exit_on_failure, dima);
    dima->status = status;

    struct dima *root = dima_from_exiting_on_failure(dima);
    root->flags |= DIMA_EXITS_ON_FAILURE;
}
