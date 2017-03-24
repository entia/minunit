# MinUnit

[![Build Status](https://travis-ci.org/entia/minunit.svg?branch=master)](https://travis-ci.org/entia/minunit)

Minunit is a minimal unit testing framework for **embedded C/C++** self-contained in a single header file. It it also has a handy serial runner.

### Features
- Single header file
- Minimal dependency (just stdio.h)
- Handy assertions types
- Short test report

### Embedded test runner
Everyone that ever worked with embedded platforms knows, that coding without proper test setup can get very annoying once the codebase grows larger.

- One command to read the test results form serial
- Connects and disconnects from serial automatically
- Easily integrates into existing build systems


## How to use it

### Installation

1. Download the latest release
2. Extract and copy the header `minunit.h` and test runner `tester` for your platform into your project's folder

### Running

1. Compile and upload code to your target microcontroller
2. run `./tester --baud 9600 --port /dev/tty.usbmodem1411` with your values or simply open your serial console 
3. You should see the test result on your screen

### Example

This is a minimal test suite written with minunit:

```C
#include <stdio.h>
#include "minunit.h"

MU_TEST(test_check) {
	mu_check(5 == 7);
}
MU_TEST_SUITE(test_suite) {
	MU_RUN_TEST(test_check);
}

int main(int argc, char *argv[]) {
	// Initialize your serial communication on your uC and map it to stdout
	// This is essential for you to see the test output
	// stdout = Serial::make_stdout();
	// See Atmega328 example in examples folder

	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
```

Expected output
```
Not yet
```

## Setup and teardown functions

One can define setup and teardown functions and configure the test suite to run
them by using the macro MU_SUITE_CONFIGURE with within a MU_TEST_SUITE
declaration.

## Assertion types

`mu_check(condition)`: will pass if the condition is evaluated to true, otherwise
it will show the condition as the error message

`mu_fail(message)`: will fail and show the message

`mu_assert(condition, message)`: will pass if the condition is true, otherwise it
will show the failed condition and the message

`mu_assert_int_eq(expected, result, message)`: it will pass if the two numbers are
equal or show their values and the message

## Authors

Samuel Vasko <samvasko@gmail.com> - embedded extensions
David Siñuela Pastor <siu.4coders@gmail.com> - original minunit
