#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include "params.h"

typedef struct arguments {
    char filename[500];
    char out_filename[504];
    // propagation + transmission delay in ns
    int user_txer_delay;
    int txer_edge_delay;
    int edge_cdn_delay;
    // memory access delay in ns
    int txer_memory_access_delay;
    int edge_memory_access_delay;
    int cdn_memory_access_delay;
    // cache size
    int txer_cache_size;
    int edge_cache_size;
    int cdn_cache_size;
    // enable caching
    int enable_edge_caching;
    int enable_transmitter_caching;
} arguments_t;

arguments_t * create_arguments();
void free_arguments(arguments_t * arguments);

#endif