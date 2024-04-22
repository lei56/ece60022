#include "cdn.h"

cdn_t * create_cdn(int index, int cache_capacity, int memory_speed) {
    cdn_t * self = (cdn_t *) malloc(sizeof(cdn_t));
    MALLOC_TEST(self, __LINE__);
    self->index = index;
    self->ingress_buffer = create_buffer(BUFFER_LENGTH*EDGES_PER_CDN);
    for (int i = 0; i < EDGES_PER_CDN; i++) {
        self->egress_buffer[i] = create_buffer(BUFFER_LENGTH);
    }
    self->cache = create_cache(cache_capacity, memory_speed);
    self->pkts_per_ts = 100;

    return self;
}

void free_cdn(cdn_t * self) {
    if (self != NULL) {
        free_buffer(self->ingress_buffer);
        for (int i = 0; i < EDGES_PER_CDN; i++) {
            free_buffer(self->egress_buffer[i]);
        }
        free_cache(self->cache);
        free(self);
    }
}