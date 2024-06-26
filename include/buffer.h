#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "params.h"

#define NULL_TEST(ptr, line_num) \
    if (ptr == NULL) \
        {printf("error: %s:%d Null pointer exception\n", __FILE__, line_num); \
        exit(0);}

#define MALLOC_TEST(ptr, line_num) \
    if (ptr == NULL) \
        {printf("error: %s:%d malloc() failed\n", __FILE__, line_num); \
        exit(0);}

typedef struct buffer {
    void ** buffer;
    int size;
    int head;
    int tail;
} buffer_t;

buffer_t * create_buffer(int size);
int buffer_push(buffer_t * self, void * element);
void * buffer_pop(buffer_t * self);
void * buffer_peek(buffer_t * self, int index);
void flush_buffer(buffer_t * self);
void free_buffer(buffer_t * self);

#endif