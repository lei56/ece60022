#include "test.h"

int test_create_buffer(buffer_t * buffer) {
    if (buffer == NULL || buffer->buffer == NULL || buffer->size != 10 || buffer->head != 0 || buffer->tail != 0) {
        return -1;
    }

    return 0;
}

int test_buffer_push(buffer_t * buffer) {
    int object = 0;
    int result = buffer_push(buffer, &object);
    if (buffer == NULL || buffer->buffer == NULL || buffer->head != 1 || buffer->tail != 0 || result != 0) {
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        result = buffer_push(buffer, &i);
    }
    if (buffer == NULL || buffer->buffer == NULL || buffer->head != 9 || buffer->tail != 0 || result != -1) {
        return -1;
    }
    
    return 0;
}

int test_buffer_pop(buffer_t * buffer) {
    int * ptr = NULL;
    int result = buffer_pop(buffer, ptr);
    if (buffer == NULL || buffer->buffer == NULL || buffer->head != 9 || buffer->tail != 1 || result != 0) {
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        result = buffer_pop(buffer, ptr);
    }
    if (buffer == NULL || buffer->buffer == NULL || buffer->head != 9 || buffer->tail != 9 || result != -1) {
        return -1;
    }

    return 0;
}

int test_buffer() {
    int buffer_tests_failed = 0;

    buffer_t * buffer = create_buffer(10);

    if (test_create_buffer(buffer) < 0) {
        buffer_tests_failed++;
    }
    if (test_buffer_push(buffer) < 0) {
        buffer_tests_failed++;
    }
    if (test_buffer_pop(buffer) < 0) {
        buffer_tests_failed++;
    }

    free_buffer(buffer);
    return buffer_tests_failed;
}

int test_create_qnode(qnode_t * node) {
    if (node == NULL || node->content_id != 0 || node->prev != NULL || node->next != NULL) {
        return -1;
    }

    return 0;
}

int test_create_cache(cache_t * cache) {
    if (cache->capacity != 5 || cache->occupancy != 0 || cache->head != NULL || cache->tail != NULL || cache->backing_memory_access_latency != 5 || cache->lock_until_time != 0) {
        return -1;
    }

    return 0;
}

int test_cache_push(cache_t * cache) {
    qnode_t * node1 = create_qnode(1);
    cache_push(cache, node1);
    if (cache->occupancy != 1 || cache->head != node1 || cache->tail != node1) {
        return -1;
    }

    qnode_t * node2 = create_qnode(2);
    cache_push(cache, node2);
    if (cache->occupancy != 2 || cache->head != node2 || cache->tail != node1) {
        return -1;
    }

    return 0;
}

int test_cache_pop(cache_t * cache) {
    qnode_t * node1 = cache_pop(cache);
    if (cache->occupancy != 1 || cache->head == NULL || cache->tail == NULL || node1->content_id != 1) {
        return -1;
    }
    free_qnode(node1);

    qnode_t * node2 = cache_pop(cache);
    if (cache->occupancy != 0 || cache->head != NULL || cache->tail != NULL || node2->content_id != 2) {
        return -1;
    }
    free_qnode(node2);

    return 0;
}

int test_cache_extract(cache_t * cache) {
    qnode_t * node3 = create_qnode(3);
    qnode_t * node4 = create_qnode(4);
    qnode_t * node5 = create_qnode(5);
    qnode_t * node6 = create_qnode(6);
    cache_push(cache, node3);
    cache_push(cache, node4);
    cache_push(cache, node5);
    cache_push(cache, node6);

    qnode_t * result = cache_extract(cache, node4);
    if (cache->occupancy != 3 || cache->head != node6 || cache->tail != node3 || result->content_id != 4) {
        return -1;
    }
    free_qnode(result);

    result = cache_extract(cache, node6);
    if (cache->occupancy != 2 || cache->head != node5 || cache->tail != node3 || result->content_id != 6) {
        return -1;
    }
    free_qnode(result);

    result = cache_extract(cache, node3);
    if (cache->occupancy != 1 || cache->head != node5 || cache->tail != node5 || result->content_id != 3) {
        return -1;
    }
    free_qnode(result);

    result = cache_extract(cache, node5);
    if (cache->occupancy != 0 || cache->head != NULL || cache->tail != NULL || result->content_id != 5) {
        return -1;
    }
    free_qnode(result);

    return 0;
}

int test_cache_check(cache_t * cache) {
    qnode_t * node7 = create_qnode(7);
    qnode_t * node8 = create_qnode(8);
    qnode_t * node9 = create_qnode(9);
    qnode_t * node10 = create_qnode(10);
    cache_push(cache, node7);
    cache_push(cache, node8);
    cache_push(cache, node9);
    cache_push(cache, node10);

    int result = cache_check(cache, 8, 0);
    if (result != 0) {
        return -1;
    }

    result = cache_check(cache, 11, 0);
    if (result <= 0) {
        return -1;
    }

    result = cache_check(cache, 11, 0);
    if (result >= 0) {
        return -1;
    }

    result = cache_check(cache, 11, 5);
    if (result != 5) {
        return -1;
    }

    return 0;
}

int test_cache_evict_and_fetch(cache_t * cache) {
    int result = cache_check(cache, 12, 5);
    if (result < 10 || cache->tail->content_id != 9 || cache->head->content_id != 12) {
        return -1;
    }

    return 0;
}

int test_memory() {
    int memory_tests_failed = 0;

    qnode_t * node = create_qnode(0);
    if (test_create_qnode(node) < 0) {
        memory_tests_failed++;
    }
    free_qnode(node);
    cache_t * cache = create_cache(5, 5);
    if (test_create_cache(cache) < 0) {
        memory_tests_failed++;
    }
    if (test_cache_push(cache) < 0) {
        memory_tests_failed++;
    }
    if (test_cache_pop(cache) < 0) {
        memory_tests_failed++;
    }
    if (test_cache_extract(cache) < 0) {
        memory_tests_failed++;
    }
    if (test_cache_check(cache) < 0) {
        memory_tests_failed++;
    }
    if (test_cache_evict_and_fetch(cache) < 0) {
        printf("failed 7\n");
        memory_tests_failed++;
    }

    free_cache(cache);
    return memory_tests_failed;
}

int main(int argc, char ** argv) {
    int tests_failed = 0;

    int buffer_tests_failed = test_buffer();
    if (buffer_tests_failed == 0) {
        printf("Passed buffer tests\n");
    }
    else {
        printf("Failed %d buffer tests\n", buffer_tests_failed);
    }

    int memory_tests_failed = test_memory();
    if (memory_tests_failed == 0) {
        printf("Passed memory tests\n");
    }
    else {
        printf("Failed %d memory tests\n", memory_tests_failed);
    }

    if (tests_failed == 0) {
        printf("Passed all tests\n");
    }
    else {
        printf("Failed %d tests\n", tests_failed);
    }

    return 0;
}