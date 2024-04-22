#ifndef __CDN_H__
#define __CDN_H__

#include "params.h"
#include "buffer.h"
#include "packet.h"
#include "memory.h"

typedef struct cdn {
    int index;
    // ingress buffer
    buffer_t * ingress_buffer;
    // egress buffer
    buffer_t * egress_buffer[EDGES_PER_CDN];
    // memory
    cache_t * cache;
    // packet processing speed per timeslot
    int pkts_per_ts;
} cdn_t;

cdn_t * create_cdn(int index, int cache_capacity, int memory_speed);
void free_cdn(cdn_t * self);

#endif