#include <stdio.h>

typedef enum {
    RB_OK = 0,
    RB_ERR_MEM = -1,
    RB_ERR_EMPTY = -2,
    RB_ERR_CAPACITY = -3,
    RB_ERR_INVALID_CB = -4,
} rb_status;

typedef struct {
    void *buffer; // Data buffer
    size_t element_size; // Size of each element
    size_t capacity; // Total number of elements buffer can hold
    size_t head; // Write index
    size_t tail; // Read index
    int is_full; // Flag indicating if buffer is full
} ringbuffer_t;

rb_status ringbuffer_init(ringbuffer_t *rbuf, size_t element_size, size_t capacity);
rb_status ringbuffer_resize(ringbuffer_t *rbuf, size_t new_capacity);
void ringbuffer_write(ringbuffer_t *rbuf, void *data);
rb_status ringbuffer_read(ringbuffer_t *rbuf, void *data);
void *ringbuffer_last_borrowed(ringbuffer_t *rbuf);
void ringbuffer_free(ringbuffer_t *rbuf);
void *ringbuffer_first_borrowed(ringbuffer_t *rbuf);
rb_status ringbuffer_peek_last(ringbuffer_t *rbuf, void *data);
rb_status ringbuffer_peek_first(ringbuffer_t *rbuf, void *data);
rb_status ringbuffer_print(ringbuffer_t *rbuf, void (*print_func)(void *));
