#ifndef __TRANSMITTER_H__
#define __TRANSMITTER_H__

#include "params.h"
#include "buffer.h"
#include "packet.h"
#include "memory.h"
#include "links.h"

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
    // cache at transmitter if enabled
    int cache_enabled;
} transmitter_t;

transmitter_t * create_transmitter(int index, int cache_capacity, int memory_speed, int enable);
void free_transmitter(transmitter_t * self);
int transmitter_process_packets(transmitter_t * self, int timeslot);
int transmitter_process_packets_upstream(transmitter_t * self, int timeslot);
int transmitter_process_packets_downstream(transmitter_t * self, int timeslot);
int transmitter_receive_packets(transmitter_t * self, links_t * links);
int transmitter_send_packets(transmitter_t * self, links_t * links);

#endif