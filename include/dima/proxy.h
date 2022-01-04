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

/**
 * dima/proxy.h
 * ------------
 * Partial DIMA implementation that makes it easy to write proxy
 * implementations.
 *
 * The struct dima_proxy exposes an exhaustive DIMA abstraction, and only
 * requires clients to implement a single function (dima_invoke_proxy_fn) that
 * handles all invocations on the proxy. The invocation on the proxy is passed
 * as a struct dima_invocation. This allows implementations to perform a common
 * operation on each invocation, including, but not limited to, invoking another
 * struct dima, with dima_invoke().
 *
 * Instances of struct dima_proxy are initialized with the dima_init_proxy()
 * function. An initialized proxy can then be used wherever a struct dima * is
 * needed, by using the dima_from_proxy() function.
 *
 * To pass extra data to the dima_invoke_proxy_fn, create a new struct that has
 * the dima_proxy as its first member, and then cast to your struct in the
 * dima_invoke_proxy_fn.
 */

#ifndef DIMA_PROXY_H
#define DIMA_PROXY_H

#include <dima/invocation.h>

struct dima_proxy;

typedef void *dima_invoke_proxy_fn(struct dima_proxy *proxy,
                                   const struct dima_invocation *invocation);

struct dima_proxy {
    struct dima dima;
    dima_invoke_proxy_fn *invoke_proxy_fn;
};

/**
 * Initializes the proxy to call the invoke_proxy_fn with each invocation.
 *
 * The dima->flags are based on the given flags.
 */
void dima_init_proxy(struct dima_proxy *proxy,
                     dima_invoke_proxy_fn *invoke_proxy_fn,
                     unsigned flags);

/**
 * Invokes the invocation on the proxy.
 *
 * This function behaves exactly as dima_invoke(), but can be used when it is
 * known that the dima is a proxy.
 */
void *dima_invoke_proxy(struct dima_proxy *proxy,
                        const struct dima_invocation *invocation);

static inline struct dima *dima_from_proxy(struct dima_proxy *proxy) {
    return &proxy->dima;
}

#endif /* !DIMA_PROXY_H */
