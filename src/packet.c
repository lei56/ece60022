#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

packet_t * create_packet(int src, int dst, int flow_id) {
    packet_t * pkt = (packet_t *) malloc(sizeof(packet_t));
    MALLOC_TEST(self, __LINE__);
    pkt->src = src;
    pkt->dst = dst;
    pkt->flow_id = flow_id;
    pkt->time_request_sent = 0;
    pkt->time_request_received = 0;
    pkt->time_content_sent = 0;
    pkt->time_content_received = 0;

    return pkt;
}