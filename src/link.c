#include "link.h"

link_t * create_link(int src, int dst, int capacity, int propagation_delay) {
    link_t * self = (link_t *) malloc(sizeof(link_t));
    MALLOC_TEST(self, __LINE__);
    self->src_node = src;
    self->dst_node = dst;
    self->fifo = create_buffer(capacity);
    self->propagation_delay = propagation_delay;
    
    return self;
}

void link_enqueue(link_t * self, void * element) {
    NULL_TEST(self, __LINE__);
    element->time_to_dequeue_from_link = self->propagation_delay;
    assert(buffer_push(self->fifo, element) != -1);
}

void * link_dequeue(link_t * self) {
    NULL_TEST(self, __LINE__);
    void * element = buffer_peek(self->fifo, self->fifo->tail);
    if (element != NULL && element->time_to_dequeue_from_link <= 0) {
        void * element = buffer_pop(self->fifo);
        assert(element != NULL);
        return element;
    }
    return NULL;
}

void * link_peek(link_t * self) {
    NULL_TEST(self, __LINE__);
    void * element = buffer_peek(self->fifo, self->fifo->tail);
    assert(element != NULL);
    return element;
}

void update_link(link_t * self) {
    buffer_t * buffer = self->fifo;
    int curr = buffer->tail;
    while (curr < head) {
        void * element = buffer_peek(buffer, curr);
        element->time_to_dequeue_from_link--;
        curr++;
    }

    return;
}

void free_link(link_t * self) {
    if (self != NULL) {
        free_buffer(self->fifo);
        free(self);
    }
}