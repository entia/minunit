#include "minunit.h"
#include <stdint.h>

static int32_t foo = 0;
static uint8_t bar = 0;

void test_setup() {
    foo = 7;
    bar = 4;
}

void test_teardown() {
    // Nothing
}

MU_TEST(test_check) {
    mu_check(foo == 7);
}

MU_TEST(test_check_fail) {
    mu_check(foo != 7);
}

MU_TEST(test_assert) {
    mu_assert(foo == 7, "foo should be 7");
}

MU_TEST(test_assert_fail) {
    mu_assert(foo != 7, "foo should be <> 7");
}

MU_TEST(test_assert_uint_eq) {
    mu_assert_uint8_eq(4, bar);
}

MU_TEST(test_assert_uint_eq_fail) {
    mu_assert_uint8_eq(23, bar);
}

MU_TEST(test_assert_int_eq) {
    mu_assert_int32_eq(7, foo);
}

MU_TEST(test_assert_int_eq_fail) {
    mu_assert_int32_eq(1, foo);
}


MU_TEST(test_mu_assert_double_close) {
    mu_assert_double_close(4.3, 4.3003, 0.1);
}

MU_TEST(test_mu_assert_double_close_fail) {
    mu_assert_double_close(4.32, 4.3003, 0.01);
}

MU_TEST(test_fail) {
    mu_fail("Fail now!");
}

MU_TEST(test_confirm) {
    mu_confirm("Everything ok?");
}


MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_check);
    MU_RUN_COND_TEST(COOL_TESTS, test_assert);
    MU_RUN_TEST(test_assert_uint_eq);
    MU_RUN_TEST(test_assert_int_eq);
    MU_RUN_TEST(test_mu_assert_double_close);

    MU_RUN_TEST(test_check_fail);
    MU_RUN_TEST(test_assert_fail);
    MU_RUN_TEST(test_assert_uint_eq_fail);
    MU_RUN_TEST(test_assert_int_eq_fail);
    MU_RUN_TEST(test_mu_assert_double_close_fail);
    MU_RUN_COND_TEST(BONUS_TESTS, test_assert_fail);
    
    MU_RUN_TEST(test_fail);
    MU_RUN_TEST(test_confirm);
}

MU_INIT();

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
