#include "transmitter.h"

transmitter_t * create_transmitter(int index, int cache_capacity, int memory_speed, int enable) {
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
    self->cache_enabled = enable;

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

int transmitter_process_packets(transmitter_t * self, int timeslot) {
    transmitter_process_packets_upstream(self, timeslot);
    transmitter_process_packets_downstream(self, timeslot);
    return 0;
}

int transmitter_process_packets_upstream(transmitter_t * self, int timeslot) {
    int pkts_sent = 0;
    packet_t * pkt = NULL;
    
    // check if packet exists in ingress buffer
    pkt = buffer_peek(self->upstream_ingress_buffer, self->upstream_ingress_buffer->tail);
    while (pkt != NULL && pkts_sent < self->pkts_per_ts) {
        if (self->cache_enabled) {
            // determine if content id stored in packet is in cache
            int cache_time = cache_peek(self->cache, pkt->content_id, timeslot);
            // cache hit
            if (cache_time == timeslot) {
                pkt = buffer_pop(self->upstream_ingress_buffer);
                pkt->time_content_sent = cache_time;
                // place into downstream buffer due to cache hit
                int user_id = pkt->src % USERS_PER_TXER;
                buffer_push(self->downstream_egress_buffer[user_id], pkt);
            }
            // cache miss
            else {
                // take packet from upstream ingress buffer
                pkt = buffer_pop(self->upstream_ingress_buffer);
                // place packet into upstream egress buffer
                buffer_push(self->upstream_egress_buffer, pkt);
            }

            // update loop variables
            pkts_sent++;
            pkt = NULL;
        }
        else {
            // take packet from upstream ingress buffer
            pkt = buffer_pop(self->upstream_ingress_buffer);
            // place packet into upstream egress buffer
            buffer_push(self->upstream_egress_buffer, pkt);

            // update loop variables
            pkts_sent++;
            pkt = NULL;
        }
    }

    return pkts_sent;
}

int transmitter_process_packets_downstream(transmitter_t * self, int timeslot) {
    int pkts_sent = 0;
    packet_t * pkt = NULL;
    // check if packet exists in ingress buffer
    pkt = buffer_peek(self->downstream_ingress_buffer, self->downstream_ingress_buffer->tail);
    while (pkt != NULL && pkts_sent < self->pkts_per_ts) {
        // cache packet contents
        if (self->cache_enabled){ 
            cache_check(self->cache, pkt->content_id, timeslot);
        }
        // take packet from downstream ingress buffer
        pkt = buffer_pop(self->downstream_ingress_buffer);
        // place packet into egress buffer
        int user_id = pkt->src % USERS_PER_TXER;
        buffer_push(self->downstream_egress_buffer[user_id], pkt);
        // update loop variables
        pkts_sent++;
        pkt = NULL;
    }
    
    return pkts_sent;
}

int transmitter_receive_packets(transmitter_t * self, links_t * links) {
    // receive on upstream
    for (int i = 0; i < NUM_OF_USERS; i++) {
        packet_t * pkt = NULL;
        link_t * link = links->user_to_transmitter_link[i][self->index];
        // check link occupancy
        pkt = buffer_peek(link->fifo, link->fifo->tail);
        if (pkt != NULL) {
            // dequeue packet from link
            pkt = buffer_pop(link->fifo);
            // place into ingress queue
            buffer_push(self->upstream_ingress_buffer, pkt);
        }
    }

    // receive on downstream
    int edge_id = self->index / TXERS_PER_EDGE;
    packet_t * pkt = NULL;
    link_t * link = links->edge_to_transmitter_link[edge_id][self->index];
    // check link occupancy
    pkt = buffer_peek(link->fifo, link->fifo->tail);
    if (pkt != NULL) {
        // dequeue packet from link
        pkt = buffer_pop(link->fifo);
        // place into ingress queue
        buffer_push(self->downstream_ingress_buffer, pkt);
    }

    return 0;
}

int transmitter_send_packets(transmitter_t * self, links_t * links) {
    // send on upstream
    packet_t * pkt = NULL;
    buffer_t * buffer = self->upstream_egress_buffer;
    // check buffer occupancy
    pkt = buffer_peek(buffer, buffer->tail);
    if (pkt != NULL) {
        // dequeue packet from buffer
        pkt = buffer_pop(buffer);
        // place onto link
        int edge_id = self->index / TXERS_PER_EDGE;
        link_t * link = links->transmitter_to_edge_link[self->index][edge_id];
        buffer_push(link->fifo, pkt);
    }
    

    // send on downstream
    for (int i = 0; i < USERS_PER_TXER; i++) {
        pkt = NULL;
        buffer = self->downstream_egress_buffer[i];
        pkt = buffer_peek(buffer, buffer->tail);
        // check buffer occupancy
        if (pkt != NULL) {
            // dequeue packet from buffer
            pkt = buffer_pop(buffer);
            // place onto link
            int user_id = self->index * 2 + i;
            link_t * link = links->transmitter_to_user_link[self->index][user_id];
            buffer_push(link->fifo, pkt);
        }
    }

    return 0;
}