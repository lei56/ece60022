#include "edge.h"

edge_t * create_edge(int index, int cache_capacity, int memory_speed) {
    edge_t * self = (edge_t *) malloc(sizeof(edge_t));
    MALLOC_TEST(self, __LINE__);
    self->index = index;
    self->upstream_ingress_buffer = create_buffer(BUFFER_LENGTH*TXERS_PER_EDGE);
    self->downstream_ingress_buffer = create_buffer(BUFFER_LENGTH*TXERS_PER_EDGE);
    self->upstream_egress_buffer = create_buffer(BUFFER_LENGTH*TXERS_PER_EDGE);
    for (int i = 0; i < TXERS_PER_EDGE; i++) {
        self->downstream_egress_buffer[i] = create_buffer(BUFFER_LENGTH);
    }
    self->cache = create_cache(cache_capacity, memory_speed);
    self->pkts_per_ts = 10;

    return self;
}

void free_edge(edge_t * self) {
    if (self != NULL) {
        free_buffer(self->upstream_ingress_buffer);
        free_buffer(self->downstream_ingress_buffer);
        free_buffer(self->upstream_egress_buffer);
        for (int i = 0; i < TXERS_PER_EDGE; i++) {
            free_buffer(self->downstream_egress_buffer[i]);
        }
        free_cache(self->cache);
        free(self);
    }
}