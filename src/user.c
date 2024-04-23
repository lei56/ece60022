#include "user.h"

user_t * create_user(int index) {
    user_t * self = (user_t *) malloc(sizeof(user_t));
    MALLOC_TEST(self, __LINE__);
    self->index = index;
    self->flow = NULL;
    self->pkts_sent = 0;
    self->pkts_received = 0;
    self->avg_content_delay = 0;

    return self;
}

void free_user(user_t * user) {
    if (user != NULL) {
        if (user->flow != NULL) {
            free(user->flow);
        }
        free(user);
    }
}

int user_receive_packets(user_t * self, links_t * links, int timeslot) {
    int txer_id = self->index / USERS_PER_TXER;
    packet_t * pkt = NULL;
    link_t * link = links->transmitter_to_user_link[txer_id][self->index];
    // check link occupancy
    pkt = buffer_peek(link->fifo, link->fifo->tail);
    if (pkt != NULL) {
        // dequeue packet from link
        pkt = buffer_pop(link->fifo);
        // process received packet
        pkt->time_content_received = timeslot;
        int pkt_in_flight_time = pkt->time_content_received - pkt->time_request_sent;
        self->avg_content_delay = (self->avg_content_delay * self->pkts_received + pkt_in_flight_time) / (self->pkts_received + 1);
        self->pkts_received++;
        free(pkt);
        return 1;
    }

    return 0;
}

int user_send_packets(user_t * self, links_t * links, int timeslot, int load) {
    // generate packet if rolling below load
    int random_number = rand() % 100;
    if (random_number < load) {
        // send packet if flow has packets remaining available
        flow_t * flow = self->flow;
        if (flow != NULL && self->pkts_sent < flow->flow_size) {
            int content_id = rand() % flow->content_ids + self->index;
            packet_t * pkt = create_packet(self->index, 0, flow->flow_id, content_id);
            // update statistics
            pkt->time_request_sent = timeslot;
            self->pkts_sent++;
            // place onto link
            int txer_id = self->index / USERS_PER_TXER;
            link_t * link = links->user_to_transmitter_link[self->index][txer_id];
            buffer_push(link->fifo, pkt);
        }    
    }

    return 0;
}