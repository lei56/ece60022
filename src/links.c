#include "links.h"
#include <params.h>

links_t * create_links()
{
    links_t * self = (links_t *) malloc(sizeof(links_t));
    MALLOC_TEST(self, __LINE__);

    for (int i = 0; i < NUM_OF_USERS; ++i) {
        for (int j = 0; j < NUM_OF_TXERS; ++j) {
            self->user_to_transmitter_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    for (int i = 0; i < NUM_OF_TXERS; ++i) {
        for (int j = 0; j < NUM_OF_EDGES; ++j) {
            self->transmitter_to_edge_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    for (int i = 0; i < NUM_OF_EDGES; ++i) {
        for (int j = 0; j < NUM_OF_CDNS; ++j) {
            self->edge_to_cdn_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    for (int i = 0; i < NUM_OF_CDNS; ++i) {
        for (int j = 0; j < NUM_OF_EDGES; ++j) {
            self->cdn_to_edge_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    for (int i = 0; i < NUM_OF_EDGES; ++i) {
        for (int j = 0; j < NUM_OF_TXERS; ++j) {
            self->edge_to_transmitter_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    for (int i = 0; i < NUM_OF_TXERS; ++i) {
        for (int j = 0; j < NUM_OF_USERS; ++j) {
            self->transmitter_to_user_link[i][j] = create_link(i, j, LINK_CAPACITY);
        }
    }

    return self;
}

void free_links(links_t * self)
{
    if (self != NULL) {
        for (int i = 0; i < NUM_OF_USERS; ++i) {
            for (int j = 0; j < NUM_OF_TXERS; ++j) {
                free_link(self->user_to_transmitter_link[i][j]);
            }
        }

        for (int i = 0; i < NUM_OF_TXERS; ++i) {
            for (int j = 0; j < NUM_OF_EDGES; ++j) {
                free_link(self->transmitter_to_edge_link[i][j]);
            }
        }

        for (int i = 0; i < NUM_OF_EDGES; ++i) {
            for (int j = 0; j < NUM_OF_CDNS; ++j) {
                free_link(self->edge_to_cdn_link[i][j]);
            }
        }

        for (int i = 0; i < NUM_OF_CDNS; ++i) {
            for (int j = 0; j < NUM_OF_EDGES; ++j) {
                free_link(self->cdn_to_edge_link[i][j]);
            }
        }

        for (int i = 0; i < NUM_OF_EDGES; ++i) {
            for (int j = 0; j < NUM_OF_TXERS; ++j) {
                free_link(self->edge_to_transmitter_link[i][j]);
            }
        }

        for (int i = 0; i < NUM_OF_TXERS; ++i) {
            for (int j = 0; j < NUM_OF_USERS; ++j) {
                free_link(self->transmitter_to_user_link[i][j]);
            }
        }

        free(self);
    }
}