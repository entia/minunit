/*
 * Copyright (c) 2012 David Siñuela Pastor, siu.4coders@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include "minunit_config.h"

/*  Misc. counters */
extern int minunit_run;
extern int minunit_assert;
extern int minunit_fail;

static int minunit_status = 0;

/*  Test setup and teardown function pointers */
static void (*minunit_setup)(void) = NULL;
static void (*minunit_teardown)(void) = NULL;

/**
 * @brief      Helper for defining test function
 * @param      method_name  The method name
 */
#define MU_TEST(method_name) static void method_name()

/**
 * @brief      Helper for defining a suite
 * @param      method_name  The method name
 */
#define MU_TEST_SUITE(suite_name) void suite_name()

/**
 * @brief      Declares the extern variables that store information about test
 * @note 	   This should be called only once generally from the main test function
 */
#define MU_INIT() \
	int minunit_run = 0;\
	int minunit_assert = 0;\
	int minunit_fail = 0;\

/**
 * @brief      Protects the macro insides form the outside world
 * @param      block  The block to be in separate scope
 */
#define MU__SAFE_BLOCK(block) do {\
	block\
} while(0)

/**
 * @brief      Run a suite (collection of tests)
 * @param      suite_name  The suite name (should be void f(void))
 */
#define MU_RUN_SUITE(suite_name) MU__SAFE_BLOCK(\
	MU_PRINTF("\r\nSuite: %s\r\n", #suite_name);\
	suite_name();\
	minunit_setup = NULL;\
	minunit_teardown = NULL;\
)

/**
 * @brief      Defines functions that will run before and after each test in suite
 * @param      setup_fun     The setup fun
 * @param      teardown_fun  The teardown fun
 */
#define MU_SUITE_CONFIGURE(setup_fun, teardown_fun) MU__SAFE_BLOCK(\
	minunit_setup = setup_fun;\
	minunit_teardown = teardown_fun;\
)

/**
 * @brief      Run a function as test
 * @param      test  The test function (should be void f(void))
 */
#define MU_RUN_TEST(test) MU__SAFE_BLOCK(\
	if (minunit_setup) (*minunit_setup)();\
	minunit_status = 0;\
	MU_PRINTF("\r\nTest: "#test"\r\n");\
	test();\
	minunit_run++;\
	if(!minunit_status) MU_PRINTF("  OK\r\n");\
	fflush(stdout);\
	if (minunit_teardown) (*minunit_teardown)();\
)

/**
 * @brief      Print summary statistics about the test
 */
#define MU_REPORT() MU__SAFE_BLOCK(\
	MU_PRINTF("\r\n\r\nTotal: %d tests, %d assertions, %d failures\r\n", minunit_run, minunit_assert, minunit_fail);\
)

/**
 * @brief      [Private] Master assert
 * @param      test     The test to be preformed
 * @param      message  The format string, pretend this is a printf
 * @param      ...      Values for the pretend printf beforehand
 */
#define __MU_ASSERT(test, message, ...)  MU__SAFE_BLOCK(\
	minunit_assert++;\
	if (!(test)) {\
		MU_PRINTF("  Fail: \""#test"\" at %s:%d\r\n  Reason: "message"\r\n", __FILE__, __LINE__, __VA_ARGS__);\
		minunit_status = 1;\
		minunit_fail++;\
		return;\
	}\
)

/**
 * @brief      Check if value is truthy
 * @param      test     The value to be checked
 */
#define mu_check(test) __MU_ASSERT(test, "%s", "Test failed")

/**
 * @brief      Just fail
 * @param      message  The message that will be printed
 */
#define mu_fail(message) __MU_ASSERT(0, "%s", message)

/**
 * @brief      Check if the value is truthy print message on fail
 * @param      test     The value to be checked
 * @param      message  The message that will be printed on error
 */
#define mu_assert(test, message) __MU_ASSERT(test, "%s", message)

/**
 * @brief      Check two ints for equality
 * @param      expected  The expected value
 * @param      result    The computed value
 */
#define mu_assert_int_eq(expected, result) MU__SAFE_BLOCK(\
	__MU_ASSERT(\
		expected == result, \
		"%d expected but was %d", \
		expected, result \
	);\
)

/**
 * @brief      Check two ints for equality
 * @param      expected  The expected value
 * @param      result    The computed value
 */
#define mu_assert_long_int_eq(expected, result) MU__SAFE_BLOCK(\
	__MU_ASSERT(\
		expected == result, \
		"%ld expected but was %ld", \
		expected, result \
	);\
)

/**
 * @brief      Check if two floats are similar
 * @param      expected  The expected value
 * @param      result    The computed value
 * @param      epsilon   The maximum allowable difference between inputs
 */
#define mu_assert_float_close(expected, result, epsilon) MU__SAFE_BLOCK(\
	float diff = fabs((expected) - (result)); \
	__MU_ASSERT( \
		diff < (epsilon), \
		"Difference of %f not within %f, %f !~= %f", \
		 diff, epsilon, \
		(expected), (result) \
	);\
)

/**
 * @brief      Check if two doubles are similar
 * @param      expected  The expected value
 * @param      result    The computed value
 * @param      epsilon   The maximum allowable difference between inputs
 */
#define mu_assert_double_close(expected, result, epsilon) MU__SAFE_BLOCK(\
	double diff = fabs((expected) - (result)); \
	__MU_ASSERT( \
		diff < (epsilon), \
		"Difference of %g not within %g, %g !~= %g", \
		 diff, epsilon, \
		(expected), (result) \
	);\
)

/**
 * @brief      Prompt user to verify something, they choose 'y' or 'n'
 * @param      message  The message to be showed to a user (generally a question)
 */
#define mu_confirm(message) MU__SAFE_BLOCK(\
	MU_PRINTF("  %s\r\n  y for yes, any key for no:\r\n", message);\
	fflush(stdout);\
	fflush(stdin);\
	char mu_input;\
	do { mu_input = getchar(); } while(mu_input != 'y' && mu_input != 'n');\
	__MU_ASSERT(mu_input == 'y', "%s", "  "message);\
)

#ifdef __cplusplus
}
#endif

#endif /* __MINUNIT_H__ */
