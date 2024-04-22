#ifndef __TEST_H__
#define __TEST_H__

#include <dirent.h>
#include <errno.h>
#include "params.h"
#include "buffer.h"
#include "memory.h"

int test_create_buffer(buffer_t * buffer);
int test_buffer_push(buffer_t * buffer);
int test_buffer_pop(buffer_t * buffer);
int test_buffer();

int test_create_qnode(qnode_t * node);
int test_create_cache(cache_t * cache);
int test_cache_push(cache_t * cache);
int test_cache_pop(cache_t * cache);
int test_cache_extract(cache_t * cache);
int test_cache_check(cache_t * cache);
int test_cache_evict_and_fetch(cache_t * cache);
int test_memory();

#endif