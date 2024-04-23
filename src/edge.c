#include "edge.h"

edge_t * create_edge(int index, int cache_capacity, int memory_speed, int enabled) {
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
    self->cache_enabled = enabled;

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

int edge_process_packets(edge_t * self, int timeslot) {
    edge_process_packets_upstream(self, timeslot);
    edge_process_packets_downstream(self, timeslot);
    return 0;
}

int edge_process_packets_upstream(edge_t * self, int timeslot) {
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
                int txer_id = pkt->src % (TXERS_PER_EDGE * USERS_PER_TXER) / USERS_PER_TXER;
                buffer_push(self->downstream_egress_buffer[txer_id], pkt);
            }
            // cache miss
            else {
                // take packet from upstream ingresss buffer
                pkt = buffer_pop(self->upstream_ingress_buffer);
                // place packet into upstream egress buffer
                buffer_push(self->upstream_egress_buffer, pkt);
            }

            // update loop variables
            pkts_sent++;
            pkt = buffer_peek(self->upstream_ingress_buffer, self->upstream_ingress_buffer->tail);
        }
        else {
            // take packet from upstream ingresss buffer
            pkt = buffer_pop(self->upstream_ingress_buffer);
            // place packet into upstream egress buffer
            buffer_push(self->upstream_egress_buffer, pkt);

            // update loop variables
            pkts_sent++;
            pkt = buffer_peek(self->upstream_ingress_buffer, self->upstream_ingress_buffer->tail);
        }
    }

    return pkts_sent;
}

int edge_process_packets_downstream(edge_t * self, int timeslot) {
    int pkts_sent = 0;
    packet_t * pkt = NULL;
    // check if packet exists in ingress buffer
    pkt = buffer_peek(self->downstream_ingress_buffer, self->downstream_ingress_buffer->tail);
    while (pkt != NULL && pkts_sent < self->pkts_per_ts) {
        // cache packet contents
        if (self->cache_enabled){ 
            cache_check(self->cache, pkt->content_id, timeslot);
        }
        // take packet from ingress buffer
        pkt = buffer_pop(self->downstream_ingress_buffer);
        // place packet into egress buffer
        int txer_id = pkt->src % (TXERS_PER_EDGE * USERS_PER_TXER) / USERS_PER_TXER;
        buffer_push(self->downstream_egress_buffer[txer_id], pkt);
        // update loop variables
        pkts_sent++;
        pkt = buffer_peek(self->downstream_ingress_buffer, self->downstream_ingress_buffer->tail);
    }

    return pkts_sent;
}

int edge_receive_packets(edge_t * self, links_t * links) {
    // receive on upstream
    for (int i = 0; i < NUM_OF_TXERS; i++) {
        packet_t * pkt = NULL;
        link_t * link = links->transmitter_to_edge_link[i][self->index];
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
    packet_t * pkt = NULL;
    link_t * link = links->cdn_to_edge_link[0][self->index];
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

int edge_send_packets(edge_t * self, links_t * links) {
    // send on upstream
    packet_t * pkt = NULL;
    buffer_t * buffer = self->upstream_egress_buffer;
    // check buffer occupancy
    pkt = buffer_peek(buffer, buffer->tail);
    if (pkt != NULL) {
        // dequeue packet from buffer
        pkt = buffer_pop(buffer);
        // place onto link
        link_t * link = links->edge_to_cdn_link[self->index][0];
        buffer_push(link->fifo, pkt);
    }
    

    // send on downstream
    for (int i = 0; i < TXERS_PER_EDGE; i++) {
        pkt = NULL;
        buffer = self->downstream_egress_buffer[i];
        // check buffer occupancy
        pkt = buffer_peek(buffer, buffer->tail);
        if (pkt != NULL) {
            // dequeue packet from buffer
            pkt = buffer_pop(buffer);
            // place onto link
            int txer_id = self->index * 10 + i;
            link_t * link = links->edge_to_transmitter_link[self->index][txer_id];
            buffer_push(link->fifo, pkt);
        }
    }

    return 0;
}