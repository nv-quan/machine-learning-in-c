#include "test_math.h"

#include <check.h>
#include <custom_math.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "utils.h"

START_TEST(test_double_eq) {
  double a = 1.23456;
  double b = 1.23457;
  double c = 1.234561;
  double d = 1.234559;
  ck_assert(!double_eq(a, b));
  ck_assert(double_eq(a, c));
  ck_assert(double_eq(c, d));
}
END_TEST

START_TEST(test_dot_product) {
  double a[] = {1.5, 2.9, 10.21};
  double b[] = {7, -12.2, 0.11122};
  double result = -23.7444438;
  double actual = dot_product(a, b, 3);

  ck_assert(double_eq(actual, result));
}
END_TEST

START_TEST(test_vec_add) {
  double a[] = {1.5, 2.9, 10.21};
  double b[] = {7, -12.2, 0.11122};
  double expected_result[] = {8.5, -9.3, 10.32122};
  double result[3];
  int i;

  vec_add(result, a, b, 3);
  for (i = 0; i < 3; i++) {
    ck_assert(double_eq(result[i], expected_result[i]));
  }
}
END_TEST

START_TEST(test_vec_mul) {
  double vec[] = {1.5, 2.9, 10.21};
  double s = 2.5;
  double expected_result[] = {3.75, 7.25, 25.525};
  double result[3];
  int i;

  vec_mul(result, vec, s, 3);
  for (i = 0; i < 3; i++) {
    ck_assert(double_eq(result[i], expected_result[i]));
  }
}
END_TEST

START_TEST(test_mat_mul) {
  double mata[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb[] = {3.1, 4.2,  -9.5, 5.3,  5.4, 4.8, -11, -14.2, -15.7, -18.9,
                   9.4, 8.42, 8.53, 2.51, 7.4, 8.5, 9.6, 8.9,   5.9,   -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  size_t s1 = 2, s2 = 5, s3 = 4;
  double result[8];
  int i;

  mat_mul(result, mata, matb, s1, s2, s3);
  for (i = 0; i < 8; ++i) {
    ck_assert(double_eq(result[i], expected[i]));
  }
}
END_TEST

START_TEST(test_mat_add) {
  double mata[] = {1, 2, 3, 4};
  double matb[] = {2, 3, 4, 5};
  double result[4];
  double expected[] = {3, 5, 7, 9};
  int i;

  mat_add(result, mata, matb, 2, 2);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(result[i], expected[i]));
  }
}
END_TEST

START_TEST(test_mat_transpose1x8) {
  double mat[] = {1, 2, 3, 4, 5, 6, 7, 8};
  double expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int i;

  mat_transpose(mat, 1, 8);
  for (i = 0; i < 8; ++i) {
    ck_assert(double_eq(mat[i], expected[i]));
  }
}
END_TEST

START_TEST(test_mat_transpose3x5) {
  double mat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  double expected[] = {1, 6, 11, 2, 7, 12, 3, 8, 13, 4, 9, 14, 5, 10, 15};
  int i;

  mat_transpose(mat, 3, 5);
  for (i = 0; i < 15; ++i) {
    ck_assert(double_eq(mat[i], expected[i]));
  }
}
END_TEST

START_TEST(test_mat_transpose1x1) {
  double mat[] = {1};
  double expected[] = {1};

  mat_transpose(mat, 1, 1);
  ck_assert(double_eq(mat[0], expected[0]));
}
END_TEST

START_TEST(test_mmat_mul_ok) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb_val[] = {3.1,   4.2,   -9.5,  5.3, 5.4,  4.8,  -11,
                       -14.2, -15.7, -18.9, 9.4, 8.42, 8.53, 2.51,
                       7.4,   8.5,   9.6,   8.9, 5.9,  -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  Mat *mata = mat_creat_from_val(mata_val, 2, 5),
      *matb = mat_creat_from_val(matb_val, 5, 4),
      *mat_expected = mat_creat_from_val(expected, 2, 4);
  int i, j;
  double a, b;

  ck_assert_ptr_nonnull(mmat_mul(mata, matb));
  ck_assert_int_eq(mata->row, 2);
  ck_assert_int_eq(mata->col, 4);
  for (i = 0; i < mata->row; ++i) {
    for (j = 0; j < mata->col; ++j) {
      a = mat_get(mata, i, j);
      b = mat_get(mat_expected, i, j);
      ck_assert_msg(double_eq(a, b),
                    "Get %d %d of mata: %lf, mat_expected: %lf", i, j, a, b);
    }
  }
  mat_destr(mata);
  mat_destr(matb);
  mat_destr(mat_expected);
}
END_TEST

START_TEST(test_mmat_mul_null) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  Mat *mata = mat_creat_from_val(mata_val, 2, 5);
  Mat *res = mmat_mul(mata, NULL);

  ck_assert_ptr_null(res);
  /* Should leave mat a alone */
  ck_assert_uint_eq(mata->row, 2);
  ck_assert_uint_eq(mata->col, 5);
  mat_destr(mata);
}
END_TEST

START_TEST(test_mmat_mul_wrong_size) {
  Mat *mata = mat_creat(2, 3);
  Mat *matb = mat_creat(2, 4);

  ck_assert_ptr_null(mmat_mul(mata, matb));

  mat_destr(mata);
  mat_destr(matb);
}
END_TEST

START_TEST(test_mmat_add_ok) {
  double mata_val[] = {1, 2, 3, 4}, matb_val[] = {2, 3, 4, 5}, result[4],
         expected_val[] = {3, 5, 7, 9};
  Mat *a = mat_creat_from_val(mata_val, 2, 2),
      *b = mat_creat_from_val(matb_val, 2, 2),
      *expected = mat_creat_from_val(expected_val, 2, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_add(a, b));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 2);
  for (i = 0; i < a->col; ++i) {
    for (j = 0; j < a->row; ++j) {
      ck_assert(double_eq(mat_get(expected, 2, 2), mat_get(a, 2, 2)));
    }
  }
  mat_destr(a);
  mat_destr(b);
  mat_destr(expected);
}
END_TEST

START_TEST(test_mmat_add_null) {
  int i;
  double mata_val[] = {1, 2, 3, 4};
  Mat *mata = mat_creat_from_val(mata_val, 2, 2);
  Mat *res = mmat_add(mata, NULL);

  ck_assert_ptr_null(res);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(mata->val[i], mata_val[i]));
  }
  mat_destr(mata);
}
END_TEST

START_TEST(test_mmat_add_wrong_size) {
  Mat *a = mat_creat(2, 2);
  Mat *b = mat_creat(3, 3);

  ck_assert_ptr_null(mmat_add(a, b));

  mat_destr(a);
  mat_destr(b);
}
END_TEST

START_TEST(test_mmat_transpose_ok) {
  double mata_val[] = {1, 2, 3, 4, 5, 6};
  double expected_val[] = {1, 4, 2, 5, 3, 6};
  Mat *a = mat_creat_from_val(mata_val, 2, 3),
      *expected = mat_creat_from_val(expected_val, 3, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_transpose(a));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 3);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(mat_get(expected, i, j), mat_get(a, i, j)));
    }
  }
  mat_destr(a);
  mat_destr(expected);
}
END_TEST

START_TEST(test_mmat_transpose_null) {
  Mat *a, *res;

  a = mat_creat(2, 2);
  ck_assert_ptr_nonnull(a);
  res = mmat_transpose(a);
  ck_assert_ptr_null(res);
  ck_assert_ptr_nonnull(a);
  mat_destr(a);
}
END_TEST

START_TEST(test_mmat_transpose_large) {
  double mata_val[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  double expected_val[] = {1,  6,  11, 16, 2,  7,  12, 17, 3,  8,
                           13, 18, 4,  9,  14, 19, 5,  10, 15, 20};
  Mat *a = mat_creat_from_val(mata_val, 4, 5),
      *expected = mat_creat_from_val(expected_val, 5, 4);
  Mat *result = mmat_transpose(a);
  size_t i, j;

  ck_assert_ptr_nonnull(result);
  ck_assert_uint_eq(a->col, 4);
  ck_assert_uint_eq(a->row, 5);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(mat_get(expected, i, j), mat_get(a, i, j)));
    }
  }
  mat_destr(a);
  mat_destr(expected);
}
END_TEST

START_TEST(test_mmat_times_ok) {
  double mata_val[] = {1, 2, 3, 4};
  double expected_val[] = {2, 4, 6, 8};
  Mat *a = mat_creat_from_val(mata_val, 2, 2),
      *expected = mat_creat_from_val(expected_val, 2, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_times(a, 2.0));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 2);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(mat_get(expected, i, j), mat_get(a, i, j)));
    }
  }
  mat_destr(a);
  mat_destr(expected);
}
END_TEST

START_TEST(test_mmat_times_null) {
  Mat *res = mmat_times(NULL, 2.0);

  ck_assert_ptr_null(res);
}
END_TEST

Suite *
math_suite(void) {
  Suite *s;
  TCase *tc_basic, *tc_vec, *tc_mat, *tc_mmat;

  s = suite_create("Math test");

  // Basic test cases
  tc_basic = tcase_create("Basic");
  tcase_add_test(tc_basic, test_double_eq);
  suite_add_tcase(s, tc_basic);

  // Vector test cases
  tc_vec = tcase_create("Vector");
  tcase_add_test(tc_vec, test_dot_product);
  tcase_add_test(tc_vec, test_vec_add);
  tcase_add_test(tc_vec, test_vec_mul);
  suite_add_tcase(s, tc_vec);

  // Matrix test cases
  tc_mat = tcase_create("Basic matrix");
  tcase_add_test(tc_mat, test_mat_mul);
  tcase_add_test(tc_mat, test_mat_add);
  tcase_add_test(tc_mat, test_mat_transpose1x8);
  tcase_add_test(tc_mat, test_mat_transpose3x5);
  tcase_add_test(tc_mat, test_mat_transpose1x1);
  suite_add_tcase(s, tc_mat);

  // Mat structure test cases
  tc_mmat = tcase_create("Mat matrix operations");
  tcase_add_test(tc_mmat, test_mmat_mul_ok);
  tcase_add_test(tc_mmat, test_mmat_mul_null);
  tcase_add_test(tc_mmat, test_mmat_mul_wrong_size);
  tcase_add_test(tc_mmat, test_mmat_add_ok);
  tcase_add_test(tc_mmat, test_mmat_add_null);
  tcase_add_test(tc_mmat, test_mmat_add_wrong_size);
  tcase_add_test(tc_mmat, test_mmat_transpose_ok);
  tcase_add_test(tc_mmat, test_mmat_transpose_null);
  tcase_add_test(tc_mmat, test_mmat_transpose_large);
  tcase_add_test(tc_mmat, test_mmat_times_ok);
  tcase_add_test(tc_mmat, test_mmat_times_null);
  suite_add_tcase(s, tc_mmat);

  return s;
}
