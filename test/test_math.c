#include "test_math.h"

#include <stdio.h>
#include <string.h>

#include "test_config.h"
#include "utils.h"

int
test_dot_product(char* test_name) {
  double a[] = {1.5, 2.9, 10.21};
  double b[] = {7, -12.2, 0.11122};
  double result = -23.7444438;
  double actual;

  strcpy(test_name, "Test dot product");
  if (!double_eq((actual = dot_product(a, b, 3)), result)) {
    fprintf(
        stderr,
        "test_dot_product: expect dot product result to be %lf, actual: %lf",
        result, actual);
    return 0;
  }
  return 1;
}

int
test_vec_add(char* test_name) {
  double a[] = {1.5, 2.9, 10.21};
  double b[] = {7, -12.2, 0.11122};
  double expected_result[] = {8.5, -9.3, 10.32122};
  double result[3];
  int i;

  strcpy(test_name, "Test vector addition");
  vec_add(result, a, b, 3);
  for (i = 0; i < 3; i++) {
    if (!double_eq(result[i], expected_result[i])) {
      fprintf(stderr, "test_vec_add: expect result[%d] to be %lf, actual: %lf",
              i, expected_result[i], result[i]);
      return 0;
    }
  }
  return 1;
}

int
test_vec_mul(char* test_name) {
  double vec[] = {1.5, 2.9, 10.21};
  double s = 2.5;
  double expected_result[] = {3.75, 7.25, 25.525};
  double result[3];
  int i;

  strcpy(test_name, "Test vector multiplication");
  vec_mul(result, vec, s, 3);
  for (i = 0; i < 3; i++) {
    if (!double_eq(result[i], expected_result[i])) {
      fprintf(stderr, "test_vec_mul: expect result[%d] to be %lf, actual: %lf",
              i, expected_result[i], result[i]);
      return 0;
    }
  }
  return 1;
}
