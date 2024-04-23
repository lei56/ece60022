#ifndef __LINKS_H__
#define __LINKS_H__

#include "params.h"
#include "link.h"

typedef struct links {
    link_t * user_to_transmitter_link[NUM_OF_USERS][NUM_OF_TXERS];
    link_t * transmitter_to_edge_link[NUM_OF_TXERS][NUM_OF_EDGES];
    link_t * edge_to_cdn_link[NUM_OF_EDGES][NUM_OF_CDNS];
    link_t * cdn_to_edge_link[NUM_OF_CDNS][NUM_OF_EDGES];
    link_t * edge_to_transmitter_link[NUM_OF_EDGES][NUM_OF_TXERS];
    link_t * transmitter_to_user_link[NUM_OF_TXERS][NUM_OF_USERS];
} links_t;

links_t * create_links(int, int, int);
void free_links(links_t *);

#endif