#include "buffer.h"

buffer_t * create_buffer(int32_t size)
{
    buffer_t * self = NULL;
    if (size > 0) {
        self = (buffer_t *) malloc(sizeof(buffer_t));
        MALLOC_TEST(self, __LINE__);
        self->buffer = (void**) malloc(size * sizeof(void*));
        MALLOC_TEST(self->buffer, __LINE__);
        for (int i = 0; i < size; i++) {
            self->buffer[i] = NULL;
        }
        self->size = size;
        self->head = 0;
        self->tail = 0;
    }

    return self;
}

int buffer_push(buffer_t * self, void * element) {
    // next is where head will point after push
    int next = self->head + 1;
    if (next >= self->size) {
        next = 0;
    }
    // check if buffer is full
    if (next == self->tail) {
        return -1;
    }
    // place object into buffer
    self->buffer[self->head] = element;
    self->head = next;
    return 0;
}

int buffer_pop(buffer_t * self, void * element) {
    // next is where tail will point after pop
    int next = self->tail + 1;
    if (next >= self->size) {
        next = 0;
    }
    // check if buffer is empty
    if (self->head == self->tail) {
        return -1;
    }
    // retrieve element from buffer
    element = self->buffer[self->tail];
    self->tail = next;
    return 0;
}

int buffer_peek(buffer_t * self, int32_t index, void * element) {
    // check if index request is valid
    if (self == NULL || index >= self->size || (index >= self->head && index <= self->tail)) {
        return -1;
    }
    // retrieve element from buffer
    element = self->buffer[index];
    return 0;
}

void flush_buffer(buffer_t * self) {
    if (self != NULL && self->buffer != NULL) {
        while (self->head != self->tail) {
            void * element = NULL;
            buffer_pop(self, element);
            if (element != NULL) {
                free(element);
            }
        }
    }
}

void free_buffer(buffer_t * self) {
    if (self == NULL) {
        return;
    }
    flush_buffer(self);
    free(self->buffer);
    free(self);
}