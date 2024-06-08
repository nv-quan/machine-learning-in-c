#include "test_math.h"

#include <stdio.h>
#include <string.h>

#include "custom_math.h"
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

int
test_mat_mul(char* test_name) {
  double mata[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb[] = {3.1, 4.2,  -9.5, 5.3,  5.4, 4.8, -11, -14.2, -15.7, -18.9,
                   9.4, 8.42, 8.53, 2.51, 7.4, 8.5, 9.6, 8.9,   5.9,   -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  size_t s1 = 2, s2 = 5, s3 = 4;
  double result[8];
  int i;

  strcpy(test_name, "Test matrix multiplication");
  mat_mul(result, mata, matb, s1, s2, s3);
  for (i = 0; i < 8; ++i) {
    if (!double_eq(result[i], expected[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, expected[i], result[i]);
      return 0;
    }
  }
  return 1;
}

int
test_mat_add(char* test_name) {
  double mata[] = {1, 2, 3, 4};
  double matb[] = {2, 3, 4, 5};
  double result[4];
  double expected[] = {3, 5, 7, 9};
  int i;

  strcpy(test_name, "Test matrix addition");
  mat_add(result, mata, matb, 2, 2);
  for (i = 0; i < 4; ++i) {
    if (!double_eq(result[i], expected[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, expected[i], result[i]);
      return 0;
    }
  }
  return 1;
}

int
test_mat_transpose1x8(char* test_name) {
  double mat[] = {1, 2, 3, 4, 5, 6, 7, 8};
  double expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int i;

  strcpy(test_name, "Test matrix transpose 1 x 8");
  mat_transpose(mat, 1, 8);
  for (i = 0; i < 8; ++i) {
    if (!double_eq(mat[i], expected[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, expected[i], mat[i]);
      return 0;
    }
  }
  return 1;
}

int
test_mat_transpose3x5(char* test_name) {
  double mat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  double expected[] = {1, 6, 11, 2, 7, 12, 3, 8, 13, 4, 9, 14, 5, 10, 15};
  int i;

  strcpy(test_name, "Test matrix transpose 3 x 5");
  mat_transpose(mat, 3, 5);
  for (i = 0; i < 15; ++i) {
    if (!double_eq(mat[i], expected[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, expected[i], mat[i]);
      return 0;
    }
  }
  return 1;
}

int
test_mat_transpose1x1(char* test_name) {
  double mat[] = {1};
  double expected[] = {1};

  strcpy(test_name, "Test matrix transpose 1 x 1");
  mat_transpose(mat, 1, 1);
  if (!double_eq(mat[0], expected[0])) {
    fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n", test_name,
            0, expected[0], mat[0]);
    return 0;
  }
  return 1;
}
