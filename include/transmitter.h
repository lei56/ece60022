#ifndef __TRANSMITTER_H__
#define __TRANSMITTER_H__

#include "params.h"
#include "buffer.h"
#include "packet.h"
#include "memory.h"

typedef struct transmitter {
    int index;
    // ingress buffers
    buffer_t * upstream_ingress_buffer;
    buffer_t * downstream_ingress_buffer;
    // egress buffers
    buffer_t * upstream_egress_buffer;
    buffer_t * downstream_egress_buffer[USERS_PER_TXER];
    // memory
    cache_t * cache;
    // packet processing speed per timeslot
    int pkts_per_ts;
} transmitter_t;

transmitter_t * create_transmitter(int index, int cache_capacity, int memory_speed);
void free_transmitter(transmitter_t * self);

#endif