/* Test program */
#include <stdio.h>
#include <stdlib.h>

#include "test_io.h"
#include "test_math.h"

#define NAME_LEN 100

int
main(int argc, char **argv) {
  int (*tests[])(char *) = {
      test_make_data_loader,
      test_point_io_mem,
      test_point_io_file,
      test_point_io_file_insert_one,
      test_point_io_mem_insert_one,
      test_continuous_loader_file1,
      test_continuous_loader_file2,
      test_continuous_loader_file3,
      test_continuous_loader_file_insert_one,
      test_continuous_loader_mem1,
      test_continuous_loader_mem2,
      test_continuous_loader_mem3,
      test_double_eq,
      test_dot_product,
      test_vec_add,
      test_vec_mul,
      test_mat_mul,
      test_mat_add,
      test_mat_transpose1x1,
      test_mat_transpose1x8,
      test_mat_transpose3x5,
      test_mmat_create,
      test_mmat_create_fromval,
      test_mmat_set_and_get_elem,
      test_mmat_set_row,
      test_mmat_set_col,
      test_mmat_mul,
      test_mmat_mul_wrong_size,
      test_mmat_add,
      test_mmat_add_same_pointer,
      test_mmat_transpose,
      test_mmat_destroy,
  };

  int failed_tests[sizeof(tests) / sizeof(int (*)(void))];
  char test_name[NAME_LEN];
  int i, n, t, pass_count;

  n = sizeof(tests) / sizeof(int (*)(void));
  t = argc > 1 ? strtol(argv[1], NULL, 10) : -1;
  pass_count = 0;
  for (i = 0; i < n; ++i) {
    if (t >= 0 && t - 1 != i) continue;
    if (tests[i](test_name)) {
      pass_count++;
      failed_tests[i] = 0;
    } else {
      failed_tests[i] = 1;
    }
    printf("%d: %s...\n", i + 1, test_name);
  }
  printf("==============TEST DONE==================\n");
  printf("%d/%d tests passed\n", pass_count, n);
  if (pass_count < n) {
    printf("Failed tests: ");
    for (i = 0; i < n; ++i) {
      if (failed_tests[i]) {
        printf("%d ", i + 1);
      }
    }
  }
}
