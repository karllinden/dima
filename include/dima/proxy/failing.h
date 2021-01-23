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

/*
 * dima/proxy/failing.h
 * --------------
 * A DIMA proxy implementation that fails unconditionally, which is useful for
 * testing.
 *
 * This implementation is thread-safe.
 */

#ifndef DIMA_PROXY_FAILING_H
#define DIMA_PROXY_FAILING_H

#include <dima/proxy/proxy.h>

/**
 * Initializes the given struct dima_proxy so that it fails unconditionally.
 */
void dima_init_failing(struct dima_proxy *proxy);

#endif /* !DIMA_PROXY_FAILING_H */
