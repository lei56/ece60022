#ifndef __LINK_H__
#define __LINK_H__

#include "params.h"
#include "buffer.h"

typedef struct link {
    int src_node;
    int dst_node;
    buffer_t * fifo;
} link_t;

link_t * create_link(int, int, int);
void link_enqueue(link_t * self, void * element);
void * link_dequeue(link_t * self);
void * link_peek(link_t * self);
void free_link(link_t * self);

#endif