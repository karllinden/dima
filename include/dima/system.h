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
 * dima/system.h
 * -------------
 * Implementation of the DIMA abstraction that uses the system's memory
 * allocation functions where possible and uses fallback implementations when
 * necessary.
 *
 * The struct dima created by dima_init_system() is thread-safe.
 */

#ifndef DIMA_SYSTEM_H
#define DIMA_SYSTEM_H

#include <dima/dima.h>

/**
 * Initializes the given dima to the system's allocation functions.
 *
 * @param dima the structure to initialize
 */
void dima_init_system(struct dima *dima);

#endif /* !DIMA_SYSTEM_H */
