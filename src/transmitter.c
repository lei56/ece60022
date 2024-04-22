#include "transmitter.h"

transmitter_t * create_transmitter(int index, int cache_capacity, int memory_speed) {
    transmitter_t * self = (transmitter_t *) malloc(sizeof(transmitter_t));
    MALLOC_TEST(self, __LINE__);
    self->index = index;
    self->upstream_ingress_buffer = create_buffer(BUFFER_LENGTH*USERS_PER_TXER);
    self->downstream_ingress_buffer = create_buffer(BUFFER_LENGTH*USERS_PER_TXER);
    self->upstream_egress_buffer = create_buffer(BUFFER_LENGTH*USERS_PER_TXER);
    for (int i = 0; i < USERS_PER_TXER; i++) {
        self->downstream_egress_buffer[i] = create_buffer(BUFFER_LENGTH);
    }
    self->cache = create_cache(cache_capacity, memory_speed);
    self->pkts_per_ts = 1;

    return self;
}

void free_transmitter(transmitter_t * self) {
    if (self != NULL) {
        free_buffer(self->upstream_ingress_buffer);
        free_buffer(self->downstream_ingress_buffer);
        free_buffer(self->upstream_egress_buffer);
        for (int i = 0; i < USERS_PER_TXER; i++) {
            free_buffer(self->downstream_egress_buffer[i]);
        }
        free_cache(self->cache);
        free(self);
    }
}