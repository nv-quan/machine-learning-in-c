#include "test_data.h"

#include <check.h>

#include "data.h"

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

START_TEST(test_mmat_destroy) {
  Mat *mat = creat_mat(3, 3);
  ck_assert_msg(mat != NULL,
                "Expected creating matrix successfully but it failed.");

  destr_mat(mat);
  /* Additional checks for destruction could be added here if destr_mat has a
   way to validate it was successful.*/
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

START_TEST(test_resize_mat_ok) {
  double initial_val[] = {1, 2, 3, 4};
  Mat *a = creat_mat_from_val(initial_val, 2, 2);

  ck_assert_uint_eq(a->capacity, 4);
  ck_assert_ptr_nonnull(resize_mat(a, 2, 4));
  ck_assert_uint_eq(a->row, 2);
  ck_assert_uint_eq(a->col, 4);
  ck_assert_uint_eq(a->capacity, 8);

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
  ck_assert_uint_eq(a->capacity, 8);

  destr_mat(a);
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

Suite *
data_suite(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("Data test");

  // Mat structure test cases
  tc = tcase_create("Matrix manimulation");
  tcase_add_test(tc, test_mmat_create);
  tcase_add_test(tc, test_mmat_create_fromval);
  tcase_add_test(tc, test_mmat_destroy);
  tcase_add_test(tc, test_mmat_set_and_get_elem);
  tcase_add_test(tc, test_set_mat_val_ok);
  tcase_add_test(tc, test_set_mat_val_null);
  tcase_add_test(tc, test_set_mat_val_wrong_length);
  tcase_add_test(tc, test_resize_mat_ok);
  tcase_add_test(tc, test_resize_mat_null);
  tcase_add_test(tc, test_resize_mat_reduce_size);
  tcase_add_test(tc, test_mmat_set_row);
  tcase_add_test(tc, test_mmat_set_col);
  suite_add_tcase(s, tc);

  return s;
}
