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
    self->pkts_per_ts = 10;

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

int cdn_process_packets(cdn_t * cdn, int timeslot) {
    int pkts_sent = 0;
    packet_t * pkt = NULL;

    // check if packet exists in ingress buffer
    pkt = buffer_peek(cdn->ingress_buffer, cdn->ingress_buffer->tail);
    while (pkt != NULL && pkts_sent < cdn->pkts_per_ts) {
        // determine if content id stored in packet is in cache
        int cache_time = cache_check(cdn->cache, pkt->content_id, timeslot);
        // cache miss when memory line is unavailable; pipeline stall
        if (cache_time < 0) {
            return pkts_sent;
        }
        // cache hit, or cache miss when memory line is available
        pkt = buffer_pop(cdn->ingress_buffer);
        pkt->time_content_sent = cache_time;
        // place packet into egress buffer
        int buffer_idx = pkt->src / (TXERS_PER_EDGE * USERS_PER_TXER);
        buffer_push(cdn->egress_buffer[buffer_idx], pkt);

        // update loop variables
        pkts_sent++;
        pkt = buffer_peek(cdn->ingress_buffer, cdn->ingress_buffer->tail);
    }
    return pkts_sent;
}

int cdn_receive_packets(cdn_t * self, links_t * links, int timeslot) {
    for (int i = 0; i < NUM_OF_EDGES; i++) {
        packet_t * pkt = NULL;
        link_t * link = links->edge_to_cdn_link[i][0];
        // check link occupancy
        pkt = buffer_peek(link->fifo, link->fifo->tail);
        if (pkt != NULL) {
            // dequeue packet from link
            pkt = buffer_pop(link->fifo);
            pkt->time_request_received = timeslot;
            // place into ingress queue
            buffer_push(self->ingress_buffer, pkt);
        }
    }

    return 0;
}

int cdn_send_packets(cdn_t * self, links_t * links, int timeslot) {
    for (int i = 0; i < NUM_OF_EDGES; i++) {
        packet_t * pkt = NULL;
        buffer_t * buffer = self->egress_buffer[i];
        // check buffer occupancy
        pkt = buffer_peek(buffer, buffer->tail);
        if (pkt != NULL) {
            // check if packet is available at current time
            if (pkt->time_content_sent <= timeslot) {
                // dequeue packet from buffer
                pkt = buffer_pop(buffer);
                // place onto link
                link_t * link = links->cdn_to_edge_link[0][i];
                buffer_push(link->fifo, pkt);
            }
        }
    }
    return 0;
}