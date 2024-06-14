#include "test_math.h"

#include <check.h>
#include <custom_math.h>
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

START_TEST(test_mmat_mul_ok) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb_val[] = {3.1,   4.2,   -9.5,  5.3, 5.4,  4.8,  -11,
                       -14.2, -15.7, -18.9, 9.4, 8.42, 8.53, 2.51,
                       7.4,   8.5,   9.6,   8.9, 5.9,  -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  Mat *mata = creat_mat_from_val(mata_val, 2, 5),
      *matb = creat_mat_from_val(matb_val, 5, 4),
      *mat_expected = creat_mat_from_val(expected, 2, 4);
  int i, j;

  ck_assert_ptr_nonnull(mmat_mul(mata, matb));
  ck_assert_int_eq(mata->row, 2);
  ck_assert_int_eq(mata->col, 4);
  for (i = 0; i < mata->row; ++i) {
    for (j = 0; j < mata->col; ++j) {
      ck_assert(double_eq(get_mat_elem(mata, i, j),
                          get_mat_elem(mat_expected, i, j)));
    }
  }
  destr_mat(mata);
  destr_mat(matb);
  destr_mat(mat_expected);
}
END_TEST

START_TEST(test_mmat_mul_null) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  Mat *mata = creat_mat_from_val(mata_val, 2, 5);
  Mat *res = mmat_mul(mata, NULL);

  ck_assert_ptr_null(res);
  /* Should leave mat a alone */
  ck_assert_uint_eq(mata->col, 2);
  ck_assert_uint_eq(mata->row, 5);
  destr_mat(mata);
}
END_TEST

START_TEST(test_mmat_mul_wrong_size) {
  Mat *mata = creat_mat(2, 3);
  Mat *matb = creat_mat(2, 4);

  ck_assert_ptr_null(mmat_mul(mata, matb));

  destr_mat(mata);
  destr_mat(matb);
}
END_TEST

START_TEST(test_mmat_add_ok) {
  double mata_val[] = {1, 2, 3, 4}, matb_val[] = {2, 3, 4, 5}, result[4],
         expected_val[] = {3, 5, 7, 9};
  Mat *a = creat_mat_from_val(mata_val, 2, 2),
      *b = creat_mat_from_val(matb_val, 2, 2),
      *expected = creat_mat_from_val(expected_val, 2, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_add(a, b));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 2);
  for (i = 0; i < a->col; ++i) {
    for (j = 0; j < a->row; ++j) {
      ck_assert(double_eq(get_mat_elem(expected, 2, 2), get_mat_elem(a, 2, 2)));
    }
  }
  destr_mat(a);
  destr_mat(b);
  destr_mat(expected);
}
END_TEST

START_TEST(test_mmat_add_null) {
  double mata_val[] = {1, 2, 3, 4};
  Mat *mata = creat_mat_from_val(mata_val, 2, 2);
  Mat *res = mmat_add(mata, NULL);

  ck_assert_ptr_null(res);
  for (i = 0; i < 8; ++i) {
    ck_assert(double_eq(mata->val[i], mata_val[i]));
  }
  destr_mat(mata);
}
END_TEST

START_TEST(test_mmat_add_wrong_size) {
  Mat *a = creat_mat(2, 2);
  Mat *b = creat_mat(3, 3);

  ck_assert_ptr_null(mmat_add(a, b));

  destr_mat(a);
  destr_mat(b);
}
END_TEST

START_TEST(test_mmat_transpose_ok) {
  double mata_val[] = {1, 2, 3, 4, 5, 6};
  double expected_val[] = {1, 4, 2, 5, 3, 6};
  Mat *a = creat_mat_from_val(mata_val, 2, 3),
      *expected = creat_mat_from_val(expected_val, 3, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_transpose(a));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 3);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(get_mat_elem(expected, i, j), get_mat_elem(a, i, j)));
    }
  }
  destr_mat(a);
  destr_mat(expected);
}
END_TEST

START_TEST(test_mmat_transpose_null) {
  Mat *res = mmat_transpose(NULL);

  ck_assert_ptr_null(res);
}
END_TEST

START_TEST(test_mmat_transpose_large) {
  double mata_val[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  double expected_val[] = {1,  6,  11, 16, 2,  7,  12, 17, 3,  8,
                           13, 18, 4,  9,  14, 19, 5,  10, 15, 20};
  Mat *a = creat_mat_from_val(mata_val, 4, 5),
      *expected = creat_mat_from_val(expected_val, 5, 4);
  Mat *result = mmat_transpose(a);
  size_t i, j;

  ck_assert_ptr_nonnull(result);
  ck_assert_uint_eq(a->col, 4);
  ck_assert_uint_eq(a->row, 5);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(get_mat_elem(expected, i, j), get_mat_elem(a, i, j)));
    }
  }
  destr_mat(a);
  destr_mat(expected);
}
END_TEST

START_TEST(test_mmat_times_ok) {
  double mata_val[] = {1, 2, 3, 4};
  double expected_val[] = {2, 4, 6, 8};
  Mat *a = creat_mat_from_val(mata_val, 2, 2),
      *expected = creat_mat_from_val(expected_val, 2, 2);
  size_t i, j;

  ck_assert_ptr_nonnull(mmat_times(a, 2.0));
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->row, 2);
  for (i = 0; i < a->row; ++i) {
    for (j = 0; j < a->col; ++j) {
      ck_assert(double_eq(get_mat_elem(expected, i, j), get_mat_elem(a, i, j)));
    }
  }
  destr_mat(a);
  destr_mat(expected);
}
END_TEST

START_TEST(test_mmat_times_null) {
  Mat *res = mmat_times(NULL, 2.0);

  ck_assert_ptr_null(res);
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

START_TEST(test_mmat_destroy) {
  Mat *mat = creat_mat(3, 3);
  ck_assert_msg(mat != NULL,
                "Expected creating matrix successfully but it failed.");

  destr_mat(mat);
  /* Additional checks for destruction could be added here if destr_mat has a
   way to validate it was successful.*/
}
END_TEST

START_TEST(test_resize_mat_ok) {
  double initial_val[] = {1, 2, 3, 4};
  Mat *a = creat_mat_from_val(initial_val, 2, 2);

  ck_assert_ptr_nonnull(resize_mat(a, 2, 4));
  ck_assert_uint_eq(a->row, 2);
  ck_assert_uint_eq(a->col, 4);

  /* Check that the initial values are preserved */
  ck_assert(double_eq(get_mat_elem(a, 0, 0), 1.0));
  ck_assert(double_eq(get_mat_elem(a, 0, 1), 2.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 0), 3.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 1), 4.0));

  /* Write to the newly allocated cells and check values */
  set_mat_elem(a, 0, 2, 5.0);
  set_mat_elem(a, 0, 3, 6.0);
  set_mat_elem(a, 1, 2, 7.0);
  set_mat_elem(a, 1, 3, 8.0);

  ck_assert(double_eq(get_mat_elem(a, 0, 2), 5.0));
  ck_assert(double_eq(get_mat_elem(a, 0, 3), 6.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 2), 7.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 3), 8.0));

  destr_mat(a);
}
END_TEST

START_TEST(test_resize_mat_null) {
  Mat *res = resize_mat(NULL, 2, 2);

  ck_assert_ptr_null(res);
}
END_TEST

START_TEST(test_resize_mat_reduce_size) {
  double initial_val[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Mat *a = creat_mat_from_val(initial_val, 2, 4);

  ck_assert_ptr_nonnull(resize_mat(a, 2, 2));
  ck_assert_uint_eq(a->row, 2);
  ck_assert_uint_eq(a->col, 2);

  // Check that the remaining values are preserved
  ck_assert(double_eq(get_mat_elem(a, 0, 0), 1.0));
  ck_assert(double_eq(get_mat_elem(a, 0, 1), 2.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 0), 5.0));
  ck_assert(double_eq(get_mat_elem(a, 1, 1), 6.0));

  destr_mat(a);
}
END_TEST

START_TEST(test_set_mat_val_ok) {
  double initial_val[] = {0, 0, 0, 0};
  double new_val[] = {1, 2, 3, 4};
  Mat *a = creat_mat_from_val(initial_val, 2, 2);
  Mat *expected = creat_mat_from_val(new_val, 2, 2);

  ck_assert_ptr_nonnull(set_mat_val(a, new_val, 4));

  for (size_t i = 0; i < a->row; ++i) {
    for (size_t j = 0; j < a->col; ++j) {
      ck_assert(double_eq(get_mat_elem(a, i, j), get_mat_elem(expected, i, j)));
    }
  }

  destr_mat(a);
  destr_mat(expected);
}
END_TEST

START_TEST(test_set_mat_val_null) {
  double new_val[] = {1, 2, 3, 4};
  Mat *res = set_mat_val(NULL, new_val, 4);

  ck_assert_ptr_null(res);
}
END_TEST

START_TEST(test_set_mat_val_wrong_length) {
  double initial_val[] = {0, 0, 0, 0};
  double new_val[] = {1, 2, 3};  // Incorrect length
  Mat *a = creat_mat_from_val(initial_val, 2, 2);

  Mat *res = set_mat_val(a, new_val, 3);  // len doesn't match mat dimensions

  ck_assert_ptr_null(res);
  // Check that original matrix remains unchanged
  for (size_t i = 0; i < a->row; ++i) {
    for (size_t j = 0; j < a->col; ++j) {
      ck_assert(double_eq(get_mat_elem(a, i, j), 0.0));
    }
  }

  destr_mat(a);
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
  tc_mmat = tcase_create("Mat matrix");
  tcase_add_test(tc_mmat, test_mmat_create);
  tcase_add_test(tc_mmat, test_mmat_create_fromval);
  tcase_add_test(tc_mmat, test_mmat_destroy);
  tcase_add_test(tc_mmat, test_mmat_set_and_get_elem);
  tcase_add_test(tc_mmat, test_mmat_mul_ok);
  tcase_add_test(tc_mmat, test_mmat_mul_null);
  tcase_add_test(tc_mmat, test_mmat_mul_wrong_size);
  tcase_add_test(tc_mmat, test_mmat_add_ok);
  tcase_add_test(tc_mmat, test_mmat_add_null);
  tcase_add_test(tc_mmat, test_mmat_add_wrong_size);
  tcase_add_test(tc_mmat, test_mmat_transpose_ok);
  tcase_add_test(tc_mmat, test_mmat_transpose_null);
  tcase_add_test(tc_mmat, test_mmat_transpose_large);
  tcase_add_test(tc_mmat, test_set_mat_val_ok);
  tcase_add_test(tc_mmat, test_set_mat_val_null);
  tcase_add_test(tc_mmat, test_set_mat_val_wrong_length);
  tcase_add_test(tc_mmat, test_resize_mat_ok);
  tcase_add_test(tc_mmat, test_resize_mat_null);
  tcase_add_test(tc_mmat, test_resize_mat_reduce_size);
  tcase_add_test(tc_mmat, test_mmat_set_row);
  tcase_add_test(tc_mmat, test_mmat_set_col);
  suite_add_tcase(s, tc_mmat);

  return s;
}
