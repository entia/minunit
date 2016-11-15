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
#include "debug.h"

/*  Maximum length of last message */
#define MINUNIT_MESSAGE_LEN 1024
/*  Float comparision difference value */
#define MINUNIT_EPSILON 1E-6

/*  Misc. counters */
extern int minunit_run;
extern int minunit_assert;
extern int minunit_fail;

static int minunit_status = 0;

/*  stdio functions */
#define MU_PRINTF printf
#define MU_GETCHAR debug_getchar

/*  Last message */
static char minunit_last_message[MINUNIT_MESSAGE_LEN];

/*  Test setup and teardown function pointers */
static void (*minunit_setup)(void) = NULL;
static void (*minunit_teardown)(void) = NULL;

/*  Definitions */
#define MU_TEST(method_name) static void method_name()
#define MU_TEST_SUITE(suite_name) static void suite_name()
#define MU_INIT() \
	int minunit_run = 0;\
	int minunit_assert = 0;\
	int minunit_fail = 0;\

#define MU__SAFE_BLOCK(block) do {\
	block\
} while(0)

/*  Run test suite and unset setup and teardown functions */
#define MU_RUN_SUITE(suite_name) MU__SAFE_BLOCK(\
	MU_PRINTF("\r\nSuite: %s\r\n", #suite_name);\
	suite_name();\
	minunit_setup = NULL;\
	minunit_teardown = NULL;\
)

/*  Configure setup and teardown functions */
#define MU_SUITE_CONFIGURE(setup_fun, teardown_fun) MU__SAFE_BLOCK(\
	minunit_setup = setup_fun;\
	minunit_teardown = teardown_fun;\
)

/*  Test runner */
#define MU_RUN_TEST(test) MU__SAFE_BLOCK(\
	if (minunit_setup) (*minunit_setup)();\
	minunit_status = 0;\
	MU_PRINTF("\r\nTest: "#test"\r\n");\
	test();\
	minunit_run++;\
	if (minunit_status) {\
		minunit_fail++;\
		MU_PRINTF("%s\r\n", minunit_last_message);\
	}\
	fflush(stdout);\
	if (minunit_teardown) (*minunit_teardown)();\
)

/*  Report */
#define MU_REPORT() MU__SAFE_BLOCK(\
	MU_PRINTF("\r\n\r\nTotal: %d tests, %d assertions, %d failures\r\n", minunit_run, minunit_assert, minunit_fail);\
)

#define __MU_ASSERT(test, message, ...)  MU__SAFE_BLOCK(\
	minunit_assert++;\
	if (!(test)) {\
		MU_PRINTF(minunit_last_message, MINUNIT_MESSAGE_LEN, "Fail: %s:%d: "#message, __FILE__, __LINE__, __VA_ARGS__);\
		minunit_status = 1;\
		return;\
	}\
)

/*  Assertions */
#define mu_check(test) __MU_ASSERT(test, "%s", "Test failed")
#define mu_fail(message) __MU_ASSERT(0, "%s", message)
#define mu_assert(test, message) __MU_ASSERT(test, "%s", message)

#define mu_assert_int_eq(expected, result) MU__SAFE_BLOCK(\
	int minunit_tmp_e = (expected);\
	int minunit_tmp_r = (result);\
	__MU_ASSERT(minunit_tmp_e == minunit_tmp_r, "%d expected but was %d", minunit_tmp_e, minunit_tmp_r);\
)

#define mu_assert_float_eq(expected, result) MU__SAFE_BLOCK(\
	float minunit_tmp_e = (expected);\
	float minunit_tmp_r = (result);\
	__MU_ASSERT(fabs(minunit_tmp_e - minunit_tmp_r) < MINUNIT_EPSILON, "%g expected but was %g", minunit_tmp_e, minunit_tmp_r);\
)

#define mu_confirm(message) MU__SAFE_BLOCK(\
	MU_PRINTF("%s\r\ny for yes, any key for no:\r\n", message);\
	fflush(stdout);\
	fflush(stdin);\
	__MU_ASSERT(MU_GETCHAR() == 'y', "%s", message);\
)

#ifdef __cplusplus
}
#endif

#endif /* __MINUNIT_H__ */
