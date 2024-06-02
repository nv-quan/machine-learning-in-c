/* Test program */
#include <stdio.h>

#include "test_io.h"
#define TEST_COUNT 1
#define NAME_LEN 100

int
main() {
  int (*tests[])(char*) = {test_io};
  char test_name[NAME_LEN];
  int i;

  for (i = 0; i < sizeof(tests) / sizeof(int (*)(void)); ++i) {
    if (tests[i](test_name)) {
      printf("%s: PASS\n", test_name);
    } else {
      printf("%s: FAIL\n", test_name);
    }
  }
}
