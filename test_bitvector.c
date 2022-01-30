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

#include <stdio.h>
#include "bitvector.h"
#include "test_utils.h"

static void
test_bitvector(void)
{
    printf("Testing bitvector initialized with all bits clear");
    print_test_separator_line();
    bitvector_t *vector = bitvector_allocate(32);
    for (size_t i = 0; i < 32; i++) {
        bool isset = bitvector_isset(vector, i);
        test(isset == false, "Expected bit %zu to be unset", i);
    }
    size_t indices[] = {5, 7, 8, 0, 10, 16};
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
        bitvector_set(vector, indices[i]);
    }

    printf("Testing bitvector set");
    print_test_separator_line();
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
        bool isset = bitvector_isset(vector, indices[i]);
        test(isset == true, "Expected bit %zu to be unset", indices[i]);
    }

    printf("Testing bitvector unset");
    print_test_separator_line();
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {
        bitvector_unset(vector, indices[i]);
    }
    for (size_t i = 0; i < 32; i++) {
        bool isset = bitvector_isset(vector, i);
        test(isset == false, "Expected bit %zu to be unset", i);
    }

    bitvector_free(vector);
}

int
main(int argc, char **argv)
{
    test_bitvector();
    return 0;

}