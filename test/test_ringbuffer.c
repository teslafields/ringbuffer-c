#define _GNU_SOURCE // NOLINT

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <stdio.h>
#include <time.h>

/* include here your files that contain test functions */
#include "ringbuffer.h"

#define PI (3.141592653589793)

struct demo_data {
    char s[20];
    int x;
    double y;
};

void print_demo_data(struct demo_data *data) {
    printf("  demo_data {\n");
    printf("    s = %s\n", data->s);
    printf("    x = %d\n", data->x);
    printf("    y = %f\n", data->y);
    printf("  }\n");
}

// NOLINTBEGIN(readability-identifier-length)
static void test_ringbuffer_creation(void **state) {
    (void) state;

    ringbuffer_t rbuffer;
    rb_status res = ringbuffer_init(&rbuffer, sizeof(struct demo_data), 3);

    assert_non_null(&rbuffer);
    assert_int_equal(res, RB_OK);

    ringbuffer_free(&rbuffer);
}

static void test_ringbuffer(void **state) {
    (void) state;

    ringbuffer_t rbuffer;

    struct demo_data data = {
        .s = {0},
        .x = 1,
        .y = PI,
    };

    rb_status res = ringbuffer_init(&rbuffer, sizeof(struct demo_data), 5);

    assert_non_null(&rbuffer);
    assert_int_equal(res, RB_OK);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    assert_int_equal(rbuffer.is_full, 0);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    assert_int_equal(rbuffer.is_full, 1);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    struct demo_data rx_data = {0};
    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 2);

    assert_int_equal(rbuffer.is_full, 0);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 4);

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 5);

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 6);

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 7);

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_OK);
    assert_int_equal(rx_data.x, 8);

    res = ringbuffer_read(&rbuffer, (void *) &rx_data);
    assert_int_equal(res, RB_ERR_EMPTY);
}

static void test_ringbuffer_peek_borrow(void **state) {
    (void) state;

    ringbuffer_t rbuffer;

    struct demo_data data = {
        .s = {0},
        .x = 1,
        .y = PI,
    };

    struct demo_data *borrowed = NULL;
    struct demo_data peeked = {0};

    rb_status res = ringbuffer_init(&rbuffer, sizeof(struct demo_data), 3);

    assert_non_null(&rbuffer);
    assert_int_equal(res, RB_OK);

    borrowed = (struct demo_data *) ringbuffer_last_borrowed(&rbuffer);
    assert_null(borrowed);

    borrowed = (struct demo_data *) ringbuffer_first_borrowed(&rbuffer);
    assert_null(borrowed);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    res = ringbuffer_peek_last(&rbuffer, &peeked);
    assert_int_equal(res, RB_OK);
    assert_int_equal(peeked.x, 1);

    res = ringbuffer_peek_first(&rbuffer, &peeked);
    assert_int_equal(res, RB_OK);
    assert_int_equal(peeked.x, 1);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    borrowed = (struct demo_data *) ringbuffer_last_borrowed(&rbuffer);
    assert_non_null(borrowed);
    assert_int_equal(borrowed->x, 2);

    borrowed = (struct demo_data *) ringbuffer_first_borrowed(&rbuffer);
    assert_non_null(borrowed);
    assert_int_equal(borrowed->x, 1);

    ringbuffer_write(&rbuffer, (void *) &data);
    data.x++;

    res = ringbuffer_peek_last(&rbuffer, &peeked);
    assert_int_equal(res, RB_OK);
    assert_int_equal(peeked.x, 3);

    res = ringbuffer_peek_first(&rbuffer, &peeked);
    assert_int_equal(res, RB_OK);
    assert_int_equal(peeked.x, 1);
}

// NOLINTEND(readability-identifier-length)

/**
 * Test runner function
 */
int main(void) {

    /**
     * Insert here your test functions
     */
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ringbuffer_creation),
        cmocka_unit_test(test_ringbuffer),
        cmocka_unit_test(test_ringbuffer_peek_borrow),
    };


    /* Run the tests */
    return cmocka_run_group_tests(tests, NULL, NULL);
}
