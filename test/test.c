/* Test program */
#include <stdio.h>

#include "test_io.h"
#include "test_math.h"
#define NAME_LEN 100

int
main() {
  int (*tests[])(char*) = {test_point_io, test_double_eq, test_dot_product,
                           test_vec_add, test_vec_mul};
  char test_name[NAME_LEN];
  int i;

  for (i = 0; i < sizeof(tests) / sizeof(int (*)(void)); ++i) {
    if (tests[i](test_name))
      printf("%d. %s: PASS\n", i + 1, test_name);
    else
      printf("%d. %s: FAIL\n", i + 1, test_name);
  }
}
