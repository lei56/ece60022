#include "arguments.h"

arguments_t * create_arguments() {
    arguments_t * arguments = (arguments_t *) malloc(sizeof(arguments_t));
    
    arguments->user_txer_delay = 1;
    arguments->txer_edge_delay = 4;
    arguments->edge_cdn_delay = 10;

    arguments->txer_memory_access_delay = 1;
    arguments->edge_memory_access_delay = 1;
    arguments->cdn_memory_access_delay = 1;

    arguments->txer_cache_size = 5;
    arguments->edge_cache_size = 50;
    arguments->cdn_cache_size = 1000;

    arguments->enable_edge_caching = 1;
    arguments->enable_transmitter_caching = 1;

    return arguments;
}

void free_arguments(arguments_t * arguments) {
    if (arguments != NULL) {
        free(arguments);
    }
}
