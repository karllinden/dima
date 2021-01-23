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

#include <dima/proxy/failing.h>

static void *invoke_failing(struct dima_proxy *proxy UNUSED,
                            const struct dima_invocation *invocation UNUSED) {
    return NULL;
}

void dima_init_failing(struct dima_proxy *proxy) {
    dima_init_proxy(proxy, invoke_failing, 0);
}
