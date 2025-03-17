#include "ringbuffer.h"

#include <stdlib.h>
#include <string.h>

// Initialize the buffer with a given capacity and element size
rb_status
ringbuffer_init(ringbuffer_t *rbuf, size_t element_size, size_t capacity) {
    rbuf->buffer = malloc(capacity * element_size);
    if (!rbuf->buffer) {
        return RB_ERR_MEM;
    }
    rbuf->element_size = element_size;
    rbuf->capacity = capacity;
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->is_full = 0;
    return RB_OK;
}

void ringbuffer_reset(ringbuffer_t *rbuf) {
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->is_full = 0;
}

// Free the circular buffer memory
void ringbuffer_free(ringbuffer_t *rbuf) { free(rbuf->buffer); }

// Resize the buffer dynamically
rb_status ringbuffer_resize(ringbuffer_t *rbuf, size_t new_capacity) {
    void *new_buffer = malloc(new_capacity * rbuf->element_size);

    if (!new_buffer) {
        return RB_ERR_MEM;
    }

    size_t size = rbuf->is_full
        ? rbuf->capacity
        : (rbuf->head - rbuf->tail + rbuf->capacity) % rbuf->capacity;

    for (size_t i = 0; i < size; i++) {
        size_t index = (rbuf->tail + i) % rbuf->capacity;
        memcpy(
            (char *)new_buffer + (i * rbuf->element_size),
            (char *)rbuf->buffer + (index * rbuf->element_size),
            rbuf->element_size
        );
    }

    free(rbuf->buffer);
    rbuf->buffer = new_buffer;
    rbuf->capacity = new_capacity;
    rbuf->head = size;
    rbuf->tail = 0;
    rbuf->is_full = (size == new_capacity);

    return RB_OK;
}

// Write an element to the buffer (overwrites old data if full)
void ringbuffer_write(ringbuffer_t *rbuf, void *data) {
    memcpy(
        (char *)rbuf->buffer + (rbuf->head * rbuf->element_size),
        data,
        rbuf->element_size
    );
    rbuf->head = (rbuf->head + 1) % rbuf->capacity;

    if (rbuf->is_full) {
        rbuf->tail = (rbuf->tail + 1) % rbuf->capacity; // Overwrite old data
    }

    rbuf->is_full = (rbuf->head == rbuf->tail);
}

// Read an element from the buffer
rb_status ringbuffer_read(ringbuffer_t *rbuf, void *data) {
    if (rbuf->head == rbuf->tail && !rbuf->is_full) {
        return RB_ERR_EMPTY; // Buffer is empty
    }

    memcpy(
        data,
        (char *)rbuf->buffer + (rbuf->tail * rbuf->element_size),
        rbuf->element_size
    );
    rbuf->tail = (rbuf->tail + 1) % rbuf->capacity;
    rbuf->is_full = 0;

    return RB_OK;
}

void *ringbuffer_last_borrowed(ringbuffer_t *rbuf) {
    if (rbuf->head == rbuf->tail && !rbuf->is_full) {
        return NULL; // Buffer is empty
    }
    size_t index = rbuf->head == 0 ? rbuf->capacity - 1 : rbuf->head - 1;
    return rbuf->buffer + (index * rbuf->element_size);
}

void *ringbuffer_first_borrowed(ringbuffer_t *rbuf) {
    if (rbuf->head == rbuf->tail && !rbuf->is_full) {
        return NULL; // Buffer is empty
    }
    return rbuf->buffer + (rbuf->tail * rbuf->element_size);
}

rb_status ringbuffer_peek_last(ringbuffer_t *rbuf, void *data) {
    if (rbuf->head == rbuf->tail && !rbuf->is_full) {
        return RB_ERR_EMPTY; // Buffer is empty
    }

    size_t index = rbuf->head == 0 ? rbuf->capacity - 1 : rbuf->head - 1;
    memcpy(
        data,
        rbuf->buffer + (index * rbuf->element_size),
        rbuf->element_size
    );

    return RB_OK;
}

rb_status ringbuffer_peek_first(ringbuffer_t *rbuf, void *data) {
    if (rbuf->head == rbuf->tail && !rbuf->is_full) {
        return RB_ERR_EMPTY; // Buffer is empty
    }

    memcpy(
        data,
        rbuf->buffer + (rbuf->tail * rbuf->element_size),
        rbuf->element_size
    );

    return RB_OK;
}

// Print the buffer contents (for debugging)
rb_status ringbuffer_print(ringbuffer_t *rbuf, void (*print_func)(void *)) {
    size_t size = rbuf->is_full
        ? rbuf->capacity
        : ((rbuf->head - rbuf->tail + rbuf->capacity) % rbuf->capacity);

    if (!print_func) {
        return RB_ERR_INVALID_CB;
    }

    for (size_t i = 0; i < size; i++) {
        size_t index = (rbuf->tail + i) % rbuf->capacity;
        print_func((char *)rbuf->buffer + (index * rbuf->element_size));
    }

    return RB_OK;
}
