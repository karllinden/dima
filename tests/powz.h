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

/* Functions to compute powers in the size_t type. */

#ifndef POWZ_H
#define POWZ_H

#include <stddef.h>

/* We assume a platform where a byte is 8 bits, although this is not required by
 * C11 standard. */
#define SIZE_BITS (sizeof(size_t) * 8)

/* Returns 3^n (mod SIZE_MAX + 1). */
size_t pow3z(size_t n);

/* Returns the multiplicative inverse of 3^n modulo SIZE_MAX + 1, i.e. the
 * number such that pow3z(n) * pow3z_inv(n) == 1. */
size_t pow3z_inv(size_t n);

#endif /* !POWZ_H */