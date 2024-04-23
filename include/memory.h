#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "params.h"
#include "buffer.h"

typedef struct qnode {
    int content_id;
    struct qnode * prev;
    struct qnode * next;
} qnode_t;

typedef struct cache {
    int capacity;
    int occupancy;
    qnode_t * head;
    qnode_t * tail;
    int backing_memory_access_latency;
    int lock_until_time;
} cache_t;

qnode_t * create_qnode(int content_id);
cache_t * create_cache(int capacity, int backing_memory_access_latency);
int cache_push(cache_t * cache, qnode_t * node);
qnode_t * cache_pop(cache_t * cache);
qnode_t * cache_extract(cache_t * cache, qnode_t * node);
int cache_check(cache_t * cache, int content_id, int current_time);
int cache_peek(cache_t * cache, int content_id, int current_time);
void cache_fetch(cache_t * cache, int content_id);
void cache_evict(cache_t * cache);
void free_qnode(qnode_t * qnode);
void free_cache(cache_t * cache);

#endif