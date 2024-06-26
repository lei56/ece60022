#ifndef __PACKET_H__
#define __PACKET_H__

#include "params.h"

typedef struct packet {
    // headers
    int src;
    int dst;
    int flow_id;
    int content_id;
    
    // statistics
    int time_request_sent;
    int time_request_received;
    int time_content_sent;
    int time_content_received;
    
    int time_to_dequeue_from_link;
    
} packet_t;

packet_t * create_packet(int src, int dst, int flow_id, int content_id);

#endif