#include "ringbuffer.h"

// Function to print integers (for testing)
void print_int(void *data) {
    printf("%d ", *(int *)data);
}

// Main function to test dynamic buffer
int main() {
    ringbuffer_t rbuffer;
    ringbuffer_init(&rbuffer, sizeof(int), 3);  // Initialize with int type and capacity of 3

    int values[] = {1, 2, 3, 4, 5, 6};

    // Writing first 3 values
    for (int i = 0; i < 3; i++) {
        ringbuffer_write(&rbuffer, &values[i]);
    }
    printf("Buffer: ");
    ringbuffer_print(&rbuffer, print_int);
    printf("\n");

    // Resize the buffer to 5 elements
    printf("Resizing buffer to 5\n");
    ringbuffer_resize(&rbuffer, 5);
    printf("Buffer: ");
    ringbuffer_print(&rbuffer, print_int);
    printf("\n");

    // Write 3 more values (some overwrite will happen)
    for (int i = 3; i < 6; i++) {
        ringbuffer_write(&rbuffer, &values[i]);
    }
    printf("Buffer: ");
    ringbuffer_print(&rbuffer, print_int);
    printf("\n");

    // Read 3 elements
    int data;
    for (int i = 0; i < 3; i++) {
        if (ringbuffer_read(&rbuffer, &data)) {
            printf("Read: %d\n", data);
        }
    }
    printf("Buffer: ");
    ringbuffer_print(&rbuffer, print_int);
    printf("\n");

    // Free memory
    ringbuffer_free(&rbuffer);
    return 0;
}
