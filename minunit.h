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

/*  Maximum length of last message */
#define MINUNIT_MESSAGE_LEN 1024
/*  Do not change */
#define MINUNIT_EPSILON 1E-12

/*  Misc. counters */
static int minunit_run __attribute__ ((unused)) = 0;
static int minunit_assert __attribute__ ((unused)) = 0;
static int minunit_fail __attribute__ ((unused)) = 0;
static int minunit_status __attribute__ ((unused)) = 0;

/*  Last message */
static char minunit_last_message[MINUNIT_MESSAGE_LEN] __attribute__ ((unused));

/*  Test setup and teardown function pointers */
static void (*minunit_setup)(void) __attribute__ ((unused)) = NULL;
static void (*minunit_teardown)(void) __attribute__ ((unused)) = NULL;

/*  Definitions */
#define MU_TEST(method_name) static void method_name()
#define MU_TEST_SUITE(suite_name) static void suite_name()

#define MU__SAFE_BLOCK(block) do {\
	block\
} while(0)

/*  Run test suite and unset setup and teardown functions */
#define MU_RUN_SUITE(suite_name) MU__SAFE_BLOCK(\
	printf("\r\nSuite: %s\r\n", #suite_name);\
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
	printf("Test: "#test"\r\r\n");\
	test();\
	minunit_run++;\
	if (minunit_status) {\
		minunit_fail++;\
		printf("\r\n%s\r\n", minunit_last_message);\
	}\
	fflush(stdout);\
	if (minunit_teardown) (*minunit_teardown)();\
)

/*  Report */
#define MU_REPORT() MU__SAFE_BLOCK(\
	printf("\r\n\r\nTotal: %d tests, %d assertions, %d failures\r\n", minunit_run, minunit_assert, minunit_fail);\
)

/*  Assertions */
#define mu_check(test) MU__SAFE_BLOCK(\
	minunit_assert++;\
	if (!(test)) {\
		snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "F %s \r\n    %s:%d: %s", __func__, __FILE__, __LINE__, #test);\
		minunit_status = 1;\
		return;\
	} else {\
		printf(". %s\r\n", __func__);\
	}\
)

#define mu_fail(message) MU__SAFE_BLOCK(\
	minunit_assert++;\
	snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "F %s \r\n    %s:%d: %s", __func__, __FILE__, __LINE__, message);\
	minunit_status = 1;\
	return;\
)

#define mu_assert(test, message) MU__SAFE_BLOCK(\
	minunit_assert++;\
	if (!(test)) {\
		snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "F %s \r\n    %s:%d: %s", __func__, __FILE__, __LINE__, message);\
		minunit_status = 1;\
		return;\
	} else {\
		printf(". %s\r\n", __func__);\
	}\
)

#define mu_assert_int_eq(expected, result) MU__SAFE_BLOCK(\
	int minunit_tmp_e;\
	int minunit_tmp_r;\
	minunit_assert++;\
	minunit_tmp_e = (expected);\
	minunit_tmp_r = (result);\
	if (minunit_tmp_e != minunit_tmp_r) {\
		snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "F %s \r\n    %s:%d: %d expected but was %d", __func__, __FILE__, __LINE__, minunit_tmp_e, minunit_tmp_r);\
		minunit_status = 1;\
		return;\
	} else {\
		printf(". %s\r\n", __func__);\
	}\
)

#define mu_assert_bit_eq(expected, reg, bitn) MU__SAFE_BLOCK(\
	unsigned int minunit_bit_r = (reg >> bitn) & 1;\
	unsigned int minunit_bit_e = (expected);\
	minunit_assert++;\
	if (minunit_bit_e != minunit_bit_r) {\
		snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, "F %s \r\n    %s:%d: %d expected but was %d", __func__, __FILE__, __LINE__, minunit_bit_e, minunit_bit_r);\
		minunit_status = 1;\
		return;\
	} else {\
		printf(". %s\r\n", __func__);\
	}\
)

#ifdef __cplusplus
}
#endif

#endif /* __MINUNIT_H__ */
