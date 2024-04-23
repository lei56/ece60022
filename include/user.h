#ifndef __USER_H__
#define __USER_H__

#include "params.h"
#include "buffer.h"
#include "packet.h"
#include "flow.h"
#include "links.h"

typedef struct user {
    int index;
    flow_t * flow;
    int pkts_sent;
    int pkts_received;
    float avg_content_delay;
} user_t;
 
user_t * create_user(int index);
void free_user(user_t * user);
int user_receive_packets(user_t * se, links_t * links, int timeslot);
int user_send_packets(user_t * self, links_t * links, int timeslot, int load);

#endif