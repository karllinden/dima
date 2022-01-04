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

/*
 * dima/system.h
 * -------------
 * Implementation of the DIMA abstraction that uses the system's memory
 * allocation functions where possible and uses fallback implementations when
 * necessary.
 *
 * A struct dima initialized with dima_init_system() is thread-safe. The struct
 * dima returned by dima_system_instance() is thread-safe.
 */

#ifndef DIMA_SYSTEM_H
#define DIMA_SYSTEM_H

#include <dima/dima.h>

/**
 * Initializes the given dima to use the system's allocation functions.
 *
 * @param dima the structure to initialize
 */
void dima_init_system(struct dima *dima);

/**
 * Returns a pointer to a global dima that uses the system's allocation
 * functions.
 *
 * The returned dima has the same behavior as if it was initialized with
 * dima_init_system.
 *
 * The returned dima is not const-qualified, because that is what the dima_*
 * functions require. However, clients may not modify the returned dima. If
 * modification is required, use dima_init_system with another struct dima.
 */
struct dima *dima_system_instance(void);

#endif /* !DIMA_SYSTEM_H */
