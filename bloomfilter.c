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
#include <stdlib.h>

#include "bloomfilter.h"
#include "murmur3.h"

static uint32_t SEEDS[] = {80430271, 89023841, 88060457, 60974549, 50009261, 87906149};

bloomfilter_t *
bloomfilter_init(size_t size)
{
    bloomfilter_t *filter;
    filter = malloc(sizeof(*filter));
    if (filter == NULL) {
        err(EXIT_FAILURE, "malloc failed");
    }
    // we init the bloom filter with size 10% greater than the requested size
    // to maintain 1% false positive rate
    size_t filter_size = 10 * size;
    filter->size = filter_size;
    // with 10% bigger filter size and 1% fp rate, the optimal number of hash
    // functions comes about to be 6
    filter->nhash = NHASH;
    filter->bitvector = bitvector_allocate(filter_size);
    return filter;
}

void
bloomfilter_put(bloomfilter_t *filter, const void *data, int len)
{
    __uint128_t hash = 0;
    for (size_t i = 0; i < NHASH; i++) {
        MurmurHash3_x64_128(data, len, SEEDS[i], &hash);
        size_t index = hash % filter->size;
        bitvector_set(filter->bitvector, index);
        hash = 0;
    }
}

bool
bloomfilter_contains(bloomfilter_t *filter, const void *data, int len)
{
    __uint128_t hash = 0;
    for (size_t i = 0; i < NHASH; i++) {
        MurmurHash3_x64_128(data, len, SEEDS[i], &hash);
        size_t index = hash % filter->size;
        bool isset = bitvector_isset(filter->bitvector, index);
        if (!isset)
            return false;
        hash = 0;
    }
    return true;
}

void
bloomfilter_free(bloomfilter_t *filter)
{
    bitvector_free(filter->bitvector);
    free(filter);
}