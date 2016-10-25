#include "minunit.h"

static int foo = 0;
static int bar = 0;

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

MU_TEST(test_assert_int_eq) {
    mu_assert_int_eq(4, bar);
}

MU_TEST(test_assert_int_eq_fail) {
    mu_assert_int_eq(5, bar);
}

MU_TEST(test_mu_assert_float_eq) {
    mu_assert_float_eq(4.3, 4.3);
}

MU_TEST(test_mu_assert_float_eq_fail) {
    mu_assert_float_eq(5, 5.3);
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
    MU_RUN_TEST(test_assert);
    MU_RUN_TEST(test_assert_int_eq);
    MU_RUN_TEST(test_mu_assert_float_eq);


    MU_RUN_TEST(test_check_fail);
    MU_RUN_TEST(test_assert_fail);
    MU_RUN_TEST(test_assert_int_eq_fail);
    MU_RUN_TEST(test_mu_assert_float_eq_fail);

    MU_RUN_TEST(test_fail);
    MU_RUN_TEST(test_confirm);
}

MU_INIT();

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}