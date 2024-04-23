#include "link.h"

link_t * create_link(int src, int dst, int capacity) {
    link_t * self = (link_t *) malloc(sizeof(link_t));
    MALLOC_TEST(self, __LINE__);
    self->src_node = src;
    self->dst_node = dst;
    self->fifo = create_buffer(capacity);
    
    return self;
}

void link_enqueue(link_t * self, void * element) {
    NULL_TEST(self, __LINE__);
    assert(buffer_push(self->fifo, element) != -1);
}

void * link_dequeue(link_t * self) {
    NULL_TEST(self, __LINE__);
    void * element = buffer_pop(self->fifo);
    assert(element != NULL);
    return element;
}

void * link_peek(link_t * self) {
    NULL_TEST(self, __LINE__);
    void * element = buffer_peek(self->fifo, self->fifo->tail);
    assert(element != NULL);
    return element;
}

void free_link(link_t * self) {
    if (self != NULL) {
        free_buffer(self->fifo);
        free(self);
    }
}