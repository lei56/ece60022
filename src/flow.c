#include "flow.h"

flow_t * create_flow(int flow_id, int flow_size, int content_id, int src, int dst) {
    flow_t * self = (flow_t *) malloc(sizeof(flow_t));
    MALLOC_TEST(self, __LINE__);
    self->flow_id = flow_id;
    self->flow_size = flow_size;
    self->content_id = content_id;
    self->src = src;
    self->dst = dst;

    return self;
}

void free_flow(flow_t * self) {
    if (self != NULL) {
        free(self);
    }
}