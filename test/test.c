/* Test program */
#include "test_io.h"
#define TEST_COUNT 1

int
main() {
  int (*tests[TEST_COUNT])(void) = {test_io};
}
