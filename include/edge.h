#ifndef __EDGE_H__
#define __EDGE_H__

#include "params.h"
#include "buffer.h"
#include "packet.h"
#include "memory.h"
#include "links.h"

typedef struct edge {
    int index;
    // ingress buffers
    buffer_t * upstream_ingress_buffer;
    buffer_t * downstream_ingress_buffer;
    // egress buffers
    buffer_t * upstream_egress_buffer;
    buffer_t * downstream_egress_buffer[TXERS_PER_EDGE];
    // memory
    cache_t * cache;
    // packet processing speed per timeslot
    int pkts_per_ts;
    // cache at edge if enabled
    int cache_enabled;
} edge_t;

edge_t * create_edge(int index, int cache_capacity, int memory_speed, int enabled);
void free_edge(edge_t * self);
int edge_process_packets(edge_t * self, int timeslot);
int edge_process_packets_upstream(edge_t * self, int timeslot);
int edge_process_packets_downstream(edge_t * self, int timeslot);
int edge_receive_packets(edge_t * self, links_t * links);
int edge_send_packets(edge_t * self, links_t * links);

#endif