/*-
 * Copyright (c) 2022 Abhinav Upadhyay <er.abhinav.upadhyay@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <err.h>
#include <string.h>
#include "bitvector.h"

bitvector_t *
bitvector_allocate(size_t size)
{
    bitvector_t *vector;
    vector = malloc(sizeof(*vector));
    if (vector == NULL)
        err(EXIT_FAILURE, "malloc failed");

    size_t nbytes = size / 8 + 1;
    vector->size = size;
    vector->vector = malloc(nbytes);
    if (vector->vector == NULL)
        err(EXIT_FAILURE, "malloc failed");
    memset(vector->vector, nbytes, 0);
    return vector;
}

void
bitvector_free(bitvector_t *vector)
{
    free(vector->vector);
    free(vector);
}

void
bitvector_set(bitvector_t *vector, size_t index)
{
    size_t byte_index = index / 8;
    size_t byte_offset = index % 8;
    vector->vector[byte_index] |= (1UL << byte_offset);
}

void
bitvector_unset(bitvector_t *vector, size_t index)
{
    size_t byte_index = index / 8;
    size_t byte_offset = index % 8;
    vector->vector[byte_index] &= ~(1UL << byte_offset);
}

bool
bitvector_isset(bitvector_t *vector, size_t index)
{
    size_t byte_index = index / 8;
    size_t byte_offset = index % 8;
    return (vector->vector[byte_index] >> byte_offset) & 1U;
}