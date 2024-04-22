#ifndef __PACKET_H__
#define __PACKET_H__

#include "params.h"

typedef struct packet {
    // headers
    int src;
    int dst;
    int flow_id;
    
    // statistics
    int time_request_sent;
    int time_request_received;
    int time_content_sent;
    int time_content_received;
    
} packet_t;

packet_t * create_packet(int src, int dst, int flow_id);

#endif