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