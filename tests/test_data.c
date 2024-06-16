#include "test_data.h"

#include <check.h>

#include "data.h"

START_TEST(test_mmat_create) {
  Mat *mat;

  mat = mat_creat(3, 3);
  ck_assert_ptr_nonnull(mat);
  ck_assert_int_eq(mat->row, 3);
  ck_assert_int_eq(mat->col, 3);
  ck_assert_ptr_nonnull(mat->val);

  mat_destr(mat);
}
END_TEST

START_TEST(test_mmat_create_fromval) {
  double matval[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  Mat *mat;
  int i;

  mat = mat_creat_from_val(matval, 3, 5);
  ck_assert_int_eq(mat->row, 3);
  ck_assert_int_eq(mat->col, 5);
  ck_assert_ptr_ne(mat->val, matval);
  ck_assert_mem_eq(mat->val, matval, sizeof(matval));

  mat_destr(mat);
}
END_TEST

START_TEST(test_mmat_destroy) {
  Mat *mat = mat_creat(3, 3);
  ck_assert_msg(mat != NULL,
                "Expected creating matrix successfully but it failed.");

  mat_destr(mat);
  /* Additional checks for destruction could be added here if mat_destr has a
   way to validate it was successful.*/
}
END_TEST

START_TEST(test_mmat_set_and_get_elem) {
  Mat *mat;
  double value = 5.5;

  mat = mat_creat(4, 4);
  ck_assert_ptr_nonnull(mat);

  mat_set(mat, 2, 2, value);
  ck_assert(double_eq(mat_get(mat, 2, 2), value));

  mat_destr(mat);
}
END_TEST

START_TEST(test_mat_set_val_ok) {
  double initial_val[] = {0, 0, 0, 0};
  double new_val[] = {1, 2, 3, 4};
  Mat *a = mat_creat_from_val(initial_val, 2, 2);
  Mat *expected = mat_creat_from_val(new_val, 2, 2);

  ck_assert_ptr_nonnull(mat_set_val(a, new_val, 4));

  for (size_t i = 0; i < a->row; ++i) {
    for (size_t j = 0; j < a->col; ++j) {
      ck_assert(double_eq(mat_get(a, i, j), mat_get(expected, i, j)));
    }
  }

  mat_destr(a);
  mat_destr(expected);
}
END_TEST

START_TEST(test_mat_set_val_null) {
  double new_val[] = {1, 2, 3, 4};
  Mat *res = mat_set_val(NULL, new_val, 4);

  ck_assert_ptr_null(res);
}
END_TEST

START_TEST(test_mat_set_val_wrong_length) {
  double initial_val[] = {0, 0, 0, 0};
  double new_val[] = {1, 2, 3};  // Incorrect length
  Mat *a = mat_creat_from_val(initial_val, 2, 2);

  Mat *res = mat_set_val(a, new_val, 3);  // len doesn't match mat dimensions

  ck_assert_ptr_null(res);
  // Check that original matrix remains unchanged
  for (size_t i = 0; i < a->row; ++i) {
    for (size_t j = 0; j < a->col; ++j) {
      ck_assert(double_eq(mat_get(a, i, j), 0.0));
    }
  }

  mat_destr(a);
}
END_TEST

START_TEST(test_mat_resize_ok) {
  double initial_val[] = {1, 2, 3, 4};
  Mat *a = mat_creat_from_val(initial_val, 2, 2);

  ck_assert_uint_eq(a->capacity, 4);
  ck_assert_ptr_nonnull(mat_resize(a, 2, 4));
  ck_assert_uint_eq(a->row, 2);
  ck_assert_uint_eq(a->col, 4);
  ck_assert_uint_eq(a->capacity, 8);

  /* Write to the newly allocated cells and check values */
  mat_set(a, 0, 2, 5.0);
  mat_set(a, 0, 3, 6.0);
  mat_set(a, 1, 2, 7.0);
  mat_set(a, 1, 3, 8.0);

  ck_assert(double_eq(mat_get(a, 0, 2), 5.0));
  ck_assert(double_eq(mat_get(a, 0, 3), 6.0));
  ck_assert(double_eq(mat_get(a, 1, 2), 7.0));
  ck_assert(double_eq(mat_get(a, 1, 3), 8.0));

  mat_destr(a);
}
END_TEST

START_TEST(test_mat_resize_null) {
  Mat *res = mat_resize(NULL, 2, 2);

  ck_assert_ptr_null(res);
}
END_TEST

START_TEST(test_mat_resize_reduce_size) {
  double initial_val[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Mat *a = mat_creat_from_val(initial_val, 2, 4);

  ck_assert_ptr_nonnull(mat_resize(a, 2, 2));
  ck_assert_uint_eq(a->row, 2);
  ck_assert_uint_eq(a->col, 2);
  ck_assert_uint_eq(a->capacity, 8);

  mat_destr(a);
}
END_TEST

START_TEST(test_mmat_set_row) {
  Mat *mat;
  double row_values[] = {1, 2, 3, 4};
  size_t row_idx = 1, i;

  mat = mat_creat(4, 4);
  ck_assert_ptr_nonnull(mat);

  mat_set_row(mat, row_idx, row_values);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(mat_get(mat, row_idx, i), row_values[i]));
  }

  mat_destr(mat);
}
END_TEST

START_TEST(test_mmat_set_col) {
  Mat *mat;
  double col_values[] = {1, 2, 3, 4};
  size_t col_idx = 1, i;

  mat = mat_creat(4, 4);
  ck_assert_ptr_nonnull(mat);

  mat_set_col(mat, col_idx, col_values);
  for (i = 0; i < 4; ++i) {
    ck_assert(double_eq(mat_get(mat, i, col_idx), col_values[i]));
  }

  mat_destr(mat);
}
END_TEST

START_TEST(test_mmat_clone) {
  double initial_val[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Mat *a = mat_creat_from_val(initial_val, 2, 4);
  Mat *b = mat_clone(a);

  ck_assert_ptr_nonnull(b);
  ck_assert_ptr_ne(a, b);
  ck_assert_ptr_ne(a->val, b->val);
  ck_assert_uint_eq(a->col, b->col);
  ck_assert_uint_eq(a->row, b->row);
  ck_assert_mem_eq(a->val, b->val, sizeof(initial_val));
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
  tcase_add_test(tc, test_mmat_clone);
  tcase_add_test(tc, test_mmat_set_and_get_elem);
  tcase_add_test(tc, test_mat_set_val_ok);
  tcase_add_test(tc, test_mat_set_val_null);
  tcase_add_test(tc, test_mat_set_val_wrong_length);
  tcase_add_test(tc, test_mat_resize_ok);
  tcase_add_test(tc, test_mat_resize_null);
  tcase_add_test(tc, test_mat_resize_reduce_size);
  tcase_add_test(tc, test_mmat_set_row);
  tcase_add_test(tc, test_mmat_set_col);
  suite_add_tcase(s, tc);

  return s;
}
