#include "arguments.h"

arguments_t * create_arguments() {
    arguments_t * arguments = (arguments_t *) malloc(sizeof(arguments_t));
    
    arguments->user_txer_delay = USER_TXER_DELAY;
    arguments->txer_edge_delay = TXER_EDGE_DELAY;
    arguments->edge_cdn_delay = EDGE_CDN_DELAY;

    arguments->txer_memory_access_delay = TXER_MEMORY_DELAY;
    arguments->edge_memory_access_delay = EDGE_MEMORY_DELAY;
    arguments->cdn_memory_access_delay = CDN_MEMORY_DELAY;

    arguments->txer_cache_size = TXER_CACHE_SIZE;
    arguments->edge_cache_size = EDGE_CACHE_SIZE;
    arguments->cdn_cache_size = CDN_CACHE_SIZE;

    arguments->enable_edge_caching = 1;
    arguments->enable_transmitter_caching = 1;

    return arguments;
}

void free_arguments(arguments_t * arguments) {
    if (arguments != NULL) {
        free(arguments);
    }
}
