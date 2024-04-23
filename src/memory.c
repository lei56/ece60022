#include "memory.h"

qnode_t * create_qnode(int content_id) {
    qnode_t * self = (qnode_t *) malloc(sizeof(qnode_t));
    MALLOC_TEST(self, __LINE__);
    self->content_id = content_id;
    self->prev = NULL;
    self->next = NULL;

    return self;
}

cache_t * create_cache(int capacity, int backing_memory_access_latency) {
    cache_t * self = (cache_t *) malloc(sizeof(cache_t));
    MALLOC_TEST(self, __LINE__);
    self->capacity = capacity;
    self->occupancy = 0;
    self->head = NULL;
    self->tail = NULL;
    self->backing_memory_access_latency = backing_memory_access_latency;
    self->lock_until_time = 0;

    return self;
}

int cache_push(cache_t * cache, qnode_t * node) {
    if (cache == NULL || node == NULL) {
        return -1;
    }

    if (cache->occupancy >= cache->capacity) {
        return -1;
    }

    // check if node is first object in cache
    if (cache->head == NULL && cache->tail == NULL) {
        cache->tail = node;
        node->next = NULL;
    }
    // if not, insert it to the head
    else {
        cache->head->prev = node;
        node->next = cache->head;
    }

    // push node to head of cache
    node->prev = NULL;
    cache->head = node;

    cache->occupancy++;

    return 0;
}

qnode_t * cache_pop(cache_t * cache) {
    if (cache == NULL || cache->occupancy == 0) {
        return NULL;
    }

    qnode_t * node = cache->tail;

    // check if node is last object in cache
    if (cache->head == cache->tail) {
        cache->head = NULL;
        cache->tail = NULL;
    }
    // if not, pop it from tail of cache
    else {
        cache->tail = node->prev;
        node->prev->next = NULL;
    }

    node->prev = NULL;
    node->next = NULL;

    cache->occupancy--;
    
    return node;
}

qnode_t * cache_extract(cache_t * cache, qnode_t * node) {
    if (cache == NULL || node == NULL || cache->occupancy == 0) {
        return NULL;
    }

    // check if node is only object in cache
    if (node == cache->head && node == cache->tail) {
        cache->head = NULL;
        cache->tail = NULL;
    }
    // check if node is head
    else if (node == cache->head) {
        cache->head = node->next;
        node->next->prev = NULL;
    }
    // check if node is tail
    else if (node == cache->tail) {
        cache->tail = node->prev;
        node->prev->next = NULL;
    }
    // otherwise, pop node from its location
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;

    // update cache statistics
    cache->occupancy--;

    return node;  
}

int cache_check(cache_t * cache, int content_id, int current_time) {
    qnode_t * curr_node = cache->head;
    while (curr_node != NULL) {
        // cache hit
        if (curr_node->content_id == content_id) {
            // bring node to front of list
            qnode_t * node = cache_extract(cache, curr_node);
            cache_push(cache, node);
            return current_time;
        }
        curr_node = curr_node->next;
    }
    // cache miss
    if (current_time < cache->lock_until_time) {
        return -1;
    }
    cache_fetch(cache, content_id);
    cache->lock_until_time = current_time + cache->backing_memory_access_latency;
    return cache->lock_until_time;
}

int cache_peek(cache_t * cache, int content_id, int current_time) {
    qnode_t * curr_node = cache->head;
    while (curr_node != NULL) {
        // cache hit
        if (curr_node->content_id == content_id) {
            // bring node to front of list
            qnode_t * node = cache_extract(cache, curr_node);
            cache_push(cache, node);
            return current_time;
        }
        curr_node = curr_node->next;
    }
    // cache miss
    return -1;
}

void cache_fetch(cache_t * cache, int content_id) {
    // cache evict if no space left
    if (cache->occupancy >= cache->capacity) {
        cache_evict(cache);
    }
    // create and push node
    qnode_t * node = create_qnode(content_id);
    cache_push(cache, node);

    return;
}

void cache_evict(cache_t * cache) {
    // pop node from end of cache
    qnode_t * node = cache_pop(cache);
    free_qnode(node);

    return;
}

void free_qnode(qnode_t * qnode) {
    free(qnode);
}

void free_cache(cache_t * cache) {
    qnode_t * curr_node = cache->head;
    while (curr_node != NULL) {
        qnode_t * next = curr_node->next;
        free(curr_node);
        curr_node = next;
    }
    free(cache);
}