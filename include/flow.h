#ifndef __FLOW_H__
#define __FLOW_H__

#include "params.h"

typedef struct flow {
    int flow_id;
    int flow_size;
    int content_ids;
    int src;
    int dst;
} flow_t;

flow_t * create_flow(int flow_id, int flow_size, int num_of_content_ids, int src, int dst);
void free_flow(flow_t * self);

#endif