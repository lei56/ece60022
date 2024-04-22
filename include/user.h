#ifndef __USER_H__
#define __USER_H__

#include "params.h"
#include "buffer.h"
#include "flow.h"

typedef struct user {
    int index;
    flow_t * flow;
    int pkts_sent;
    int pkts_received;
    float avg_content_delay;
} user_t;
 
user_t * create_user(int index);
void free_user(user_t * user);

#endif