# DIMA - Dependency Injectable Memory Allocator

## Introduction

DIMA is a dependency injectable memory allocator for C programs that want to
correctly handle failure to dynamically allocate memory, and want to demonstrate
that correctness in unit tests.
This is especially important for libraries, and to some extent embedded and
safety-critical systems.
(The latter are questionable, because they probably better stay away from
dynamic memory management entirely.)

To be dependency injectable, DIMA provides an abstraction with dynamic dispatch
that additionally gives the flexibility to change memory allocating strategies
(at runtime).
This allows users of this library to easily experiment with custom
implementations of `malloc()`, for example for devices without an operating
system.
(DIMA does not yet support building without the system's memory allocation
functions, but it should be fairly easy to make the dependency on the standard
library optional.)

## Motivation

C programs must manually manage their memory correctly, and while testing with
an analyzer (such as Valgrind) can demonstrate the correctness of some
successful program paths, it is harder to demonstrate that a program behaves
correctly in the precense of out-of-memory conditions.
As far as I know, there are at least three ways to test `malloc()` failures:

### LD_PRELOAD

Some linkers support loading additional shared libraries before loading the
libraries required by the binary.
For example GNU `ld` loads shared libraries from the `LD_PRELOAD` environment
variable first.
While this approach does not require modifying any calls to `malloc()`, it has
the drawbacks that it is system-dependent and that `malloc()` is replaced in the
entire program.
This makes it harder to inject a failure in a specific unit test.

### Malloc hooks

The GNU standard C library provides malloc hooks that can be changed at
runtime.
Unlike `LD_PRELOAD`, malloc hooks have the advantage that failures can be
injected in a unit test.
However, like `LD_PRELOAD`, this approach is system-dependent.

### Dependency injection

With dependency injection, the downsides of the previous approaches can be
mitigated.
It has the price that the calls to `malloc()` to fault-inject must be made
through some indirection, which implies both that some calls to `malloc()` must
be updated and a performance penalty.
In the case of DIMA, this amounts to calling `dima_malloc()` instead of
`malloc()`.

Dependency injection is the approach taken by many libraries, such as libcurl,
libevent and libpng.
These libraries allow function pointers to custom memory functions to be
injected.
Each library commonly has its own function pointer types for these functions,
which means that testing code will be repeated in some way.
DIMA aims to provide a memory management abstraction that is re-usable by other
libraries.

## Installation

This library can be installed with the following commands

```bash
meson build
ninja -C build
sudo ninja -C build install
```

You can then get the compiler flags and libraries through `pkg-config` with
`pkg-config --cflags dima-${X}` and `pkg-config --libs dima-${X}` with `${X}`
substituted by the major version of this library.

## Usage

Using DIMA is very straight-forward.
You only need to dependency inject a `struct dima` to where it is needed.
The library comes with a few ready-to-use implementations of `struct dima`, such
as `dima_system` (that uses the operating system's memory allocation),
`dima_env_instance()` (that allows the memory allocation to be configured from
environment variables) and `dima_exiting_on_failure` (which exits the program
on failure).
For example, this silly `range` function allows any DIMA implementation to be
used for creating the returned array:

```c
void init_range(size_t *range, size_t max_exclusive) {
    range[0] = max_exclusive;
    for (size_t i = 0; i < max_exclusive; ++i) {
        range[i + 1] = i;
    }
}

size_t *range(struct dima *dima, size_t max_exclusive) {
    if (max_exclusive == SIZE_MAX) {
        /* max_exclusive + 1 would overflow. */
        return NULL;
    }

    size_t *result = dima_mallocarray(dima, max_exlusive + 1, sizeof(*result));
    if (result != NULL) {
        init_range(result, max_exclusive);
    }
    return result;
}
```

The example also uses `dima_mallocarray(dima, nmemb, size)` which behaves like
`dima_malloc(dima, nmemb * size)` but with an overflow check, similar to how the
non-standard `reallocarray()` function works.
The above function can then be invoked with different memory allocators, for
example using:

```c
void do_something_with_range(const size_t *r) {
    /* ... */
}

void example(struct dima *dima, size_t max_exclusive) {
    size_t *r = range(dima, max_exclusive);
    do_something_with_range(r);
    dima_free(dima, r);
}

int main(void) {
    /* Use the memory allocation functions provided by the system. */
    example(&dima_system, 10);

    /* Use a DIMA that is configured with environment variables. */
    example(dima_env_instance(), 41);

    /* Decorate the system's allocator to exit with status 14 on failure. */
    struct dima_exiting_on_failure deof;
    dima_init_exiting_on_failure(&deof, &dima_system, 14);
    example(dima_from_exiting_on_failure(&deof), 24);

    return 0;
}
```

## Extending

It is possible to implement custom allocators outside DIMA.
See the instructions in `include/dima/dima.h` for how to do that.
See also the implementations that are part of this library, for example
`src/exiting_on_failure.c` and `src/malloc_and_free.h`.
