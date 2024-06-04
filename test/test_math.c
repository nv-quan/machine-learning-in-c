#include "test_math.h"

#include <stdio.h>
#include <string.h>

#include "test_config.h"
#include "test_math.h"
#include "utils.h"

int
test_double_eq(char* test_name) {
  double a = 1.23456;
  double b = 1.23457;
  double c = 1.234561;
  double d = 1.234559;

  strcpy(test_name, "Test double equality");
  if (double_eq(a, b)) {
    fprintf(stderr,
            "test_double_eq: expected %lf to not be equal to %lf, but they are",
            a, b);
    return 0;
  }

  if (!double_eq(a, c)) {
    fprintf(stderr,
            "test_double_eq: expected %lf to be equal to %lf, but they are not",
            a, c);
    return 0;
  }

  if (!double_eq(a, d)) {
    fprintf(stderr,
            "test_double_eq: expected %lf to be equal to %lf, but they are not",
            a, d);
    return 0;
  }

  return 1;
}

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
        "test_dot_product: expect dot product result to be %lf, actual: %lf\n",
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
      fprintf(stderr,
              "test_vec_add: expect result[%d] to be %lf, actual: %lf\n", i,
              expected_result[i], result[i]);
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
      fprintf(stderr,
              "test_vec_mul: expect result[%d] to be %lf, actual: %lf\n", i,
              expected_result[i], result[i]);
      return 0;
    }
  }
  return 1;
}
