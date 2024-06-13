#include <check.h>
#include <custom_math.h>
#include <stdlib.h>

#include "test_math.h"
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

START_TEST(test_mmat_create_fromval) {
  double matval[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  Mat *mata, *matb;
  int i;

  mata = creat_mat(2, 3);
  ck_assert_ptr_nonnull(mata);

  matb = creat_mat_from_val(matval, 3, 5);
  ck_assert_int_eq(mata->row, 2);
  ck_assert_int_eq(mata->col, 3);
  ck_assert_int_eq(matb->row, 3);
  ck_assert_int_eq(matb->col, 5);

  for (i = 0; i < 15; ++i) {
    ck_assert(double_eq(matb->val[i], matval[i]));
  }

  destr_mat(mata);
  destr_mat(matb);
}
END_TEST

START_TEST(test_mmat_set_and_get_elem) {
  Mat *mat;
  double value = 5.5;

  mat = creat_mat(4, 4);
  ck_assert_ptr_nonnull(mat);

  set_mat_elem(mat, 2, 2, value);
  ck_assert(double_eq(get_mat_elem(mat, 2, 2), value));

  destr_mat(mat);
}
END_TEST

START_TEST(test_mmat_mul) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb_val[] = {3.1,   4.2,   -9.5,  5.3, 5.4,  4.8,  -11,
                       -14.2, -15.7, -18.9, 9.4, 8.42, 8.53, 2.51,
                       7.4,   8.5,   9.6,   8.9, 5.9,  -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  Mat *mata = creat_mat_from_val(mata_val, 2, 5),
      *matb = creat_mat_from_val(matb_val, 5, 4), *matr = creat_mat(2, 4);
  int i;

  ck_assert(mmat_mul(matr, mata, matb) == 0);
  for (i = 0; i < 8; ++i) {
    ck_assert(double_eq(matr->val[i], expected[i]));
  }

  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
}
END_TEST

START_TEST(test_mmat_mul_wrong_size) {
  Mat *mata = creat_mat(2, 3);
  Mat *matb = creat_mat(2, 4);
  Mat *matr = creat_mat(2, 4);

  int result = mmat_mul(matr, mata, matb);

  ck_assert_msg(result,
                "Expected mmat_mul to fail due to incorrect matrix sizes, but "
                "it succeeded.");

  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
}
END_TEST

START_TEST(test_mmat_set_row) {
  Mat *mat;
  double row_values[] = {1, 2, 3, 4};
  size_t row_idx = 1, i;

  mat = creat_mat(4, 4);
  ck_assert_ptr_nonnull(mat);

  set_mat_row(mat, row_idx, row_values);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(get_mat_elem(mat, row_idx, i), row_values[i]));
  }

  destr_mat(mat);
}
END_TEST

START_TEST(test_mmat_set_col) {
  Mat *mat;
  double col_values[] = {1, 2, 3, 4};
  size_t col_idx = 1, i;

  mat = creat_mat(4, 4);
  ck_assert_ptr_nonnull(mat);

  set_mat_col(mat, col_idx, col_values);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(get_mat_elem(mat, i, col_idx), col_values[i]));
  }

  destr_mat(mat);
}
END_TEST

START_TEST(test_mmat_create) {
  Mat *mat;

  mat = creat_mat(3, 3);
  ck_assert_ptr_nonnull(mat);
  ck_assert_int_eq(mat->row, 3);
  ck_assert_int_eq(mat->col, 3);
  ck_assert_ptr_nonnull(mat->val);

  destr_mat(mat);
}
END_TEST

START_TEST(test_mmat_add) {
  double mata_val[] = {1, 2, 3, 4};
  double matb_val[] = {2, 3, 4, 5};
  double expected[] = {3, 5, 7, 9};
  Mat *mata = creat_mat_from_val(mata_val, 2, 2),
      *matb = creat_mat_from_val(matb_val, 2, 2), *matr = creat_mat(2, 2);
  int i;

  ck_assert(mmat_add(matr, mata, matb) == 0);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(matr->val[i], expected[i]));
  }

  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
}
END_TEST

START_TEST(test_mmat_add_same_pointer) {
  double mataval[] = {1, 2, 3, 4, 5, 6};
  double matbval[] = {7, 8, 9, 10, 11, 12};
  double expected_sum[] = {8, 10, 12, 14, 16, 18};
  Mat *mata = creat_mat_from_val(mataval, 3, 2);
  Mat *matb = creat_mat_from_val(matbval, 3, 2);

  ck_assert_msg(mata != NULL && matb != NULL,
                "Expected creating matrices successfully but it failed.");

  int result = mmat_add(mata, mata, matb);
  ck_assert_msg(
      result == 0,
      "Expected matrix addition operation to be successful but it failed.");

  ck_assert_msg(mata->row == 3 && mata->col == 2,
                "Wrong dimensions for result matrix.");

  for (size_t i = 0; i < mata->row; ++i) {
    for (size_t j = 0; j < mata->col; ++j) {
      ck_assert_msg(double_eq(mata->val[i * mata->col + j],
                              expected_sum[i * mata->col + j]),
                    "Expected element at (%zu,%zu) to be %lf, actual: %lf", i,
                    j, expected_sum[i * mata->col + j],
                    mata->val[i * mata->col + j]);
    }
  }

  destr_mat(mata);
  destr_mat(matb);
}
END_TEST

START_TEST(test_mmat_transpose) {
  double mat_val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  double expected[] = {1, 6, 11, 2, 7, 12, 3, 8, 13, 4, 9, 14, 5, 10, 15};
  Mat *mat = creat_mat_from_val(mat_val, 3, 5);
  int i;

  mmat_transpose(mat);
  for (i = 0; i < 15; ++i) {
    ck_assert(double_eq(mat->val[i], expected[i]));
  }

  destr_mat(mat);
}
END_TEST

START_TEST(test_mmat_destroy) {
  Mat *mat = creat_mat(3, 3);
  ck_assert_msg(mat != NULL,
                "Expected creating matrix successfully but it failed.");

  destr_mat(mat);
  // Additional checks for destruction could be added here if destr_mat has a
  // way to validate it was successful.
}
END_TEST

Suite *
math_suite(void) {
  Suite *s;
  TCase *tc_basic, *tc_vec, *tc_mat, *tc_mmat;

  s = suite_create("Mathtestaaaaaaaaaaaaaaaaa");

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
  tc_mat = tcase_create("Matrix");
  tcase_add_test(tc_mat, test_mat_mul);
  tcase_add_test(tc_mat, test_mat_add);
  tcase_add_test(tc_mat, test_mat_transpose1x8);
  tcase_add_test(tc_mat, test_mat_transpose3x5);
  tcase_add_test(tc_mat, test_mat_transpose1x1);
  suite_add_tcase(s, tc_mat);

  // Mat structure test cases
  tc_mmat = tcase_create("Mat");
  tcase_add_test(tc_mmat, test_mmat_create);
  tcase_add_test(tc_mmat, test_mmat_create_fromval);
  tcase_add_test(tc_mmat, test_mmat_destroy);
  tcase_add_test(tc_mmat, test_mmat_set_and_get_elem);
  tcase_add_test(tc_mmat, test_mmat_mul);
  tcase_add_test(tc_mmat, test_mmat_mul_wrong_size);
  tcase_add_test(tc_mmat, test_mmat_set_row);
  tcase_add_test(tc_mmat, test_mmat_set_col);
  tcase_add_test(tc_mmat, test_mmat_add);
  tcase_add_test(tc_mmat, test_mmat_transpose);
  suite_add_tcase(s, tc_mmat);

  return s;
}
