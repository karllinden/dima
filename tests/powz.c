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

#include "src/size_bits.h"

#include "powz.h"

#define ORDER_OF_3 (((size_t)1) << (SIZE_BITS - 2))

size_t pow3z(size_t n) {
    size_t result = 1;
    size_t p = 3;
    while (n != 0) {
        if (n % 2 != 0) {
            result *= p;
        }
        p *= p;
        n /= 2;
    }
    return result;
}

size_t pow3z_inv(size_t n) {
    size_t order = ORDER_OF_3;
    n %= order;
    return pow3z(order - n);
}
