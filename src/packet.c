#include "packet.h"

packet_t * create_packet(int src, int dst, int flow_id, int content_id) {
    packet_t * pkt = (packet_t *) malloc(sizeof(packet_t));
    MALLOC_TEST(pkt, __LINE__);
    pkt->src = src;
    pkt->dst = dst;
    pkt->flow_id = flow_id;
    pkt->content_id = content_id;
    pkt->time_request_sent = 0;
    pkt->time_request_received = 0;
    pkt->time_content_sent = 0;
    pkt->time_content_received = 0;
    pkt->time_to_dequeue_from_link = 0;

    return pkt;
}