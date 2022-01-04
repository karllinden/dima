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

#include <dima/proxy.h>

#include "invocations.h"
#include "test.h"

struct test_proxy {
    struct dima_proxy proxy;
    struct dima_invocation invocation;
    unsigned count;
};

/* Wrapper around a dima, which prevents dima_invoke() from short-circuiting and
 * calling dima_invoke_proxy() directly, which forces a call to the proxy
 * through the vtable. */
struct non_proxy {
    struct dima dima;
    struct dima *next;
};

static char fake_return;

static void *noop(struct dima_proxy *proxy UNUSED,
                  const struct dima_invocation *invocation UNUSED) {
    return NULL;
}

static void *invoke_test_proxy(struct dima_proxy *p,
                               const struct dima_invocation *invocation) {
    struct test_proxy *proxy = (struct test_proxy *)p;
    dima_copy_invocation(&proxy->invocation, invocation);
    proxy->count++;
    return &fake_return;
}

static void init_test_proxy(struct test_proxy *proxy) {
    dima_init_proxy(&proxy->proxy, invoke_test_proxy, 0);
    proxy->count = 0;
}

static struct non_proxy *to_non_proxy(struct dima *dima) {
    return (struct non_proxy *)dima;
}

static void non_proxy_free(struct dima *dima, void *ptr) {
    dima_free(to_non_proxy(dima)->next, ptr);
}

static void *non_proxy_alloc(struct dima *dima, size_t size) {
    return dima_alloc(to_non_proxy(dima)->next, size);
}

static void *non_proxy_alloc0(struct dima *dima, size_t size) {
    return dima_alloc0(to_non_proxy(dima)->next, size);
}

static void *non_proxy_realloc(struct dima *dima, void *ptr, size_t size) {
    return dima_realloc(to_non_proxy(dima)->next, ptr, size);
}

static void *non_proxy_alloc_array(struct dima *dima,
                                   size_t nmemb,
                                   size_t size) {
    return dima_alloc_array(to_non_proxy(dima)->next, nmemb, size);
}

static void *non_proxy_alloc_array0(struct dima *dima,
                                    size_t nmemb,
                                    size_t size) {
    return dima_alloc_array0(to_non_proxy(dima)->next, nmemb, size);
}

static void *non_proxy_realloc_array(struct dima *dima,
                                     void *ptr,
                                     size_t nmemb,
                                     size_t size) {
    return dima_realloc_array(to_non_proxy(dima)->next, ptr, nmemb, size);
}

static char *non_proxy_strdup(struct dima *dima, const char *s) {
    return dima_strdup(to_non_proxy(dima)->next, s);
}

static char *non_proxy_strndup(struct dima *dima, const char *s, size_t n) {
    return dima_strndup(to_non_proxy(dima)->next, s, n);
}

static const struct dima_vtable non_proxy_vtable = {
        non_proxy_free,
        non_proxy_alloc,
        non_proxy_alloc0,
        non_proxy_realloc,
        non_proxy_alloc_array,
        non_proxy_alloc_array0,
        non_proxy_realloc_array,
        non_proxy_strdup,
        non_proxy_strndup,
};

static void init_non_proxy(struct non_proxy *np, struct dima *next) {
    np->dima.vtable = &non_proxy_vtable;
    np->dima.flags = 0;
    np->next = next;
}

START_TEST(test_proxy_is_proxy) {
    struct dima_proxy proxy;
    dima_init_proxy(&proxy, noop, 0);
    ck_assert_int_ne(0, dima_is_proxy(dima_from_proxy(&proxy)));
}
END_TEST

START_TEST(test_exits_on_failure_if_set_in_flags) {
    struct dima_proxy proxy;
    dima_init_proxy(&proxy, noop, DIMA_EXITS_ON_FAILURE);
    ck_assert_int_ne(0, dima_exits_on_failure(dima_from_proxy(&proxy)));
}
END_TEST

START_TEST(test_does_not_exit_on_failure_if_not_set_in_flags) {
    struct dima_proxy proxy;
    dima_init_proxy(&proxy, noop, 0);
    ck_assert_int_eq(0, dima_exits_on_failure(dima_from_proxy(&proxy)));
}
END_TEST

typedef void *invoke_fn(struct test_proxy *proxy,
                        const struct dima_invocation *invocation);

static void *invoke(struct test_proxy *proxy,
                    const struct dima_invocation *invocation) {
    return dima_invoke(dima_from_proxy(&proxy->proxy), invocation);
}

static void *invoke_proxy(struct test_proxy *proxy,
                          const struct dima_invocation *invocation) {
    return dima_invoke_proxy(&proxy->proxy, invocation);
}

static void *invoke_through_non_proxy(
        struct test_proxy *proxy,
        const struct dima_invocation *invocation) {
    struct non_proxy np;
    init_non_proxy(&np, dima_from_proxy(&proxy->proxy));
    return dima_invoke(&np.dima, invocation);
}

static void test_invoke(int invocation_index, invoke_fn *invoke) {
    const struct dima_invocation *inv = invocations + invocation_index;
    struct test_proxy proxy;
    init_test_proxy(&proxy);
    void *ret = invoke(&proxy, inv);
    if (inv->function != DIMA_FREE) {
        ck_assert_ptr_eq(&fake_return, ret);
    }
    ck_assert_int_eq(1, proxy.count);
    ck_assert_int_eq(0, dima_compare_invocations(inv, &proxy.invocation));
}

START_TEST(test_invoke_works) {
    test_invoke(_i, invoke);
}
END_TEST

START_TEST(test_invoke_proxy_works) {
    test_invoke(_i, invoke_proxy);
}
END_TEST

START_TEST(test_invoke_through_non_proxy_works) {
    test_invoke(_i, invoke_through_non_proxy);
}
END_TEST

void init_test_dima(void) {
    /* The test_dima is not used so that the invoke_test_proxy function must use
     * its first argument. */
    test_dima = NULL;
}

void add_tests(Suite *suite) {
    ADD_TEST(proxy_is_proxy);
    ADD_TEST(exits_on_failure_if_set_in_flags);
    ADD_TEST(does_not_exit_on_failure_if_not_set_in_flags);
    init_invocations();
    ADD_LOOP_TEST(invoke_works, 0, N_INVOCATIONS);
    ADD_LOOP_TEST(invoke_proxy_works, 0, N_INVOCATIONS);
    ADD_LOOP_TEST(invoke_through_non_proxy_works, 0, N_INVOCATIONS);
}
