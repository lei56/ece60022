#include "user.h"

user_t * create_user(int index) {
    user_t * self = (user_t *) malloc(sizeof(user_t));
    MALLOC_TEST(self, __LINE__);
    self->index = index;
    self->flow = NULL;
    self->pkts_sent = 0;
    self->pkts_received = 0;
    self->avg_pkt_delay = 0;
}

void free_user(user_t * user) {
    if (user != NULL) {
        free(user);
    }
}