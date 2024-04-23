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

void link_enqueue(link_t * self, packet_t * packet) {
    NULL_TEST(self, __LINE__);
    packet->time_to_dequeue_from_link = self->propagation_delay;
    assert(buffer_push(self->fifo, packet) != -1);
}

packet_t * link_dequeue(link_t * self) {
    NULL_TEST(self, __LINE__);
    packet_t * packet = buffer_peek(self->fifo, self->fifo->tail);
    if (packet != NULL && packet->time_to_dequeue_from_link <= 0) {
        packet_t * packet = buffer_pop(self->fifo);
        assert(packet != NULL);
        return packet;
    }
    return NULL;
}

packet_t * link_peek(link_t * self) {
    NULL_TEST(self, __LINE__);
    packet_t * packet = buffer_peek(self->fifo, self->fifo->tail);
    assert(packet != NULL);
    return packet;
}

void update_link(link_t * self) {
    buffer_t * buffer = self->fifo;
    int curr = buffer->tail;
    while (curr < buffer->head) {
        packet_t * packet = buffer_peek(buffer, curr);
        packet->time_to_dequeue_from_link--;
        if (packet->time_to_dequeue_from_link < 0) {
            packet->time_to_dequeue_from_link = 0;
        }
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