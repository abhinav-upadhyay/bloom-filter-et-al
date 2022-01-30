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

#include "bloomfilter.h"
#include "test_utils.h"

static void
test_bloomfilter(void)
{
    printf("Testing bloomfilter put and contains");
    print_test_separator_line();
    size_t size = 1000000;
    bloomfilter_t *filter = bloomfilter_init(size);
    size_t fp_count = 0;
    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0)
            bloomfilter_put(filter, &i, sizeof(size_t));
    }
    for (size_t i = 0; i < size; i++) {
        bool contains = bloomfilter_contains(filter, &i, sizeof(i));
        if (i % 2 == 0) {
            test(contains == true, "Expected value %zu to be present in filter", i);
        } else {
            if (contains == true) {
                printf("Expected value %zu to be not present in filter, maybe false positive\n", i);
                fp_count++;
            }
        }
    }
    bloomfilter_free(filter);
    printf("Total number of false positive = %zu, percentage: %f\n", fp_count, 100.0 * fp_count / size);
}

int
main(int argc, char **argv)
{
    test_bloomfilter();
    return 0;
}