/* Test program */
#include <stdio.h>

#include "test_io.h"
#include "test_math.h"
#define NAME_LEN 100

int
main(int argc, char **argv) {
  int (*tests[])(char *) = {
      test_dot_product,
      test_vec_add,
      test_vec_mul,
      test_make_data_loader,
      test_double_eq,
      test_point_io_mem,
      test_point_io_file,
      test_continuous_loader_file1,
      test_continuous_loader_file2,
      test_continuous_loader_file3,
      test_continuous_loader_mem1,
      test_continuous_loader_mem2,
      test_continuous_loader_mem3,
  };
  char test_name[NAME_LEN];
  int i, n, t;

  n = sizeof(tests) / sizeof(int (*)(void));
  t = argc > 1 ? strtol(argv[1], NULL, 10) : -1;
  for (i = 0; i < n; ++i) {
    if (t >= 0 && t - 1 != i) continue;
    if (tests[i](test_name))
      printf("%d. %s: PASS\n", i + 1, test_name);
    else
      printf("%d. %s: FAIL\n", i + 1, test_name);
  }
}
