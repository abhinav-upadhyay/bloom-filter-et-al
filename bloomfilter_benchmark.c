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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "bloomfilter.h"

const char *FILENAME = "web3";

static void
read_file_and_index(bloomfilter_t *filter)
{
    FILE *f = fopen(FILENAME, "r");
    if (f == NULL)
        err(EXIT_FAILURE, "Failed to open file for reading");
    ssize_t bytes_read;
    char *line = NULL;
    size_t linesize = 0;
    while ((bytes_read = getline(&line, &linesize, f)) != -1) {
        line[bytes_read - 1] = 0;
        bloomfilter_put(filter, line, bytes_read);
        free(line);
        linesize = 0;
        line = NULL;
    }
    free(line);
    fclose(f);
}

static void
read_file_and_query(bloomfilter_t *filter)
{
    FILE *f = fopen(FILENAME, "r");
    if (f == NULL)
        err(EXIT_FAILURE, "Failed to open file for reading");
    ssize_t bytes_read;
    char *line = NULL;
    size_t linesize = 0;
    while ((bytes_read = getline(&line, &linesize, f)) != -1) {
        line[bytes_read - 1] = 0;
        bool contains = bloomfilter_contains(filter, line, bytes_read);
        if (contains == false) {
            printf("Expected for the filter to contain %s\n", line);
        }
        free(line);
        linesize = 0;
        line = NULL;
    }
    free(line);
    fclose(f);
}

static void
print_resource_usage(size_t filter_size)
{
    size_t vector_size = filter_size * 10;
    size_t bytes_reqd = vector_size / 8 + 1;
    printf("Memory used for %f mb\n", 1.0 * bytes_reqd / (1024 * 1024));
}

int
main(int argc, char **argv)
{
    bloomfilter_t *filter = bloomfilter_init(500000);
    read_file_and_index(filter);
    read_file_and_query(filter);
    print_resource_usage(500000);
    bloomfilter_free(filter);
    return 0;
}