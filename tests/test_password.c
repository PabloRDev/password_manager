//
// Created by Pablo RD on 5/6/25.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Example test function
static void test_sample(void **state) {
    (void) state; // unused
    assert_int_equal(42, 42);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sample),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
