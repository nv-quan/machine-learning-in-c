#include "test_math.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom_math.h"
#include "data.h"
#include "test_config.h"
#include "test_math.h"
#include "utils.h"

/* TODO: Add capacity check in all mmat tests */

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

int
test_mmat_create_fromval(char* test_name) {
  double matval[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  Mat *mata, *matb;
  int i, retval;

  strcpy(test_name, "Test Mat matrix creation from value");
  mata = creat_mat(2, 3);
  if (mata == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    retval = 0;
    goto cleanup1;
  }
  matb = creat_mat_from_val(matval, 3, 5);
  if (mata->row != 2 || mata->col != 3 || matb->row != 3 || matb->col != 5) {
    fprintf(stderr, "%s: wrong matrix size\n", test_name);
    retval = 0;
    goto cleanup2;
  }
  for (i = 0; i < 15; ++i) {
    if (!double_eq(matb->val[i], matval[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, matval[i], matb->val[i]);
      retval = 0;
      goto cleanup2;
    }
  }
  retval = 1;
cleanup2:
  destr_mat(matb);
cleanup1:
  destr_mat(mata);
  return retval;
}

int
test_mmat_set_and_get_elem(char* test_name) {
  Mat* mat;
  int retval;
  size_t i, j;
  double value = 5.5;

  strcpy(test_name, "Test Mat element set and get");
  mat = creat_mat(4, 4);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    retval = 0;
    goto cleanup;
  }

  set_mat_elem(mat, 2, 2, value);
  if (!double_eq(get_mat_elem(mat, 2, 2), value)) {
    fprintf(stderr, "%s: expect element at (2,2) to be %lf, actual: %lf\n",
            test_name, value, get_mat_elem(mat, 2, 2));
    retval = 0;
    goto cleanup;
  }

  retval = 1;
cleanup:
  destr_mat(mat);
  return retval;
}

int
test_mmat_mul(char* test_name) {
  double mata_val[] = {1.2, -4.2, 1.2, 1.8, 9.2, 4.7, -5.5, 1.3, 2.3, -3.4};
  double matb_val[] = {3.1,   4.2,   -9.5,  5.3, 5.4,  4.8,  -11,
                       -14.2, -15.7, -18.9, 9.4, 8.42, 8.53, 2.51,
                       7.4,   8.5,   9.6,   8.9, 5.9,  -50};
  double expected[] = {65.874,  48.598,  113.68, -368.596,
                       -48.561, -55.717, 25.03,  303.506};
  Mat *mata = creat_mat_from_val(mata_val, 2, 5),
      *matb = creat_mat_from_val(matb_val, 5, 4), *matr = creat_mat(2, 4);
  int i, retval;

  strcpy(test_name, "Test Mat matrix multiplication");
  memcpy((void*)mata->val, (const void*)mata_val, sizeof(mata_val));
  memcpy((void*)matb->val, (const void*)matb_val, sizeof(matb_val));
  if (mmat_mul(matr, mata, matb)) {
    fprintf(stderr, "%s: expect mmat_mul to finish successfully but it's not\n",
            test_name);
    retval = 0;
    goto cleanup;
  }
  for (i = 0; i < 8; ++i) {
    if (!double_eq(matr->val[i], expected[i])) {
      fprintf(stderr, "%s: expect result[%d] to be %lf, actual: %lf\n",
              test_name, i, expected[i], matr->val[i]);
      retval = 0;
      goto cleanup;
    }
  }
  retval = 1;

cleanup:
  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
  return retval;
}

int
test_mmat_set_row(char* test_name) {
  Mat* mat;
  int retval;
  double row_values[] = {1, 2, 3, 4};
  size_t row_idx = 1, i;

  strcpy(test_name, "Test Mat set row");
  mat = creat_mat(4, 4);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    retval = 0;
    goto cleanup;
  }

  set_mat_row(mat, row_idx, row_values);
  for (i = 0; i < 4; ++i) {
    if (!double_eq(get_mat_elem(mat, row_idx, i), row_values[i])) {
      fprintf(
          stderr, "%s: expect element at (%zu,%zu) to be %lf, actual: %lf\n",
          test_name, row_idx, i, row_values[i], get_mat_elem(mat, row_idx, i));
      retval = 0;
      goto cleanup;
    }
  }

  retval = 1;
cleanup:
  destr_mat(mat);
  return retval;
}

int
test_mmat_set_col(char* test_name) {
  Mat* mat;
  int retval;
  double col_values[] = {1, 2, 3, 4};
  size_t col_idx = 1, i;

  strcpy(test_name, "Test Mat set column");
  mat = creat_mat(4, 4);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    retval = 0;
    goto cleanup;
  }

  set_mat_col(mat, col_idx, col_values);
  for (i = 0; i < 4; ++i) {
    if (!double_eq(get_mat_elem(mat, i, col_idx), col_values[i])) {
      fprintf(
          stderr, "%s: expect element at (%zu,%zu) to be %lf, actual: %lf\n",
          test_name, i, col_idx, col_values[i], get_mat_elem(mat, i, col_idx));
      retval = 0;
      goto cleanup;
    }
  }

  retval = 1;
cleanup:
  destr_mat(mat);
  return retval;
}

int
test_mmat_create(char* test_name) {
  Mat* mat;
  int retval;

  strcpy(test_name, "Test Mat matrix creation");
  mat = creat_mat(3, 3);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    retval = 0;
  } else if (mat->row != 3 || mat->col != 3) {
    fprintf(stderr, "%s: wrong matrix size\n", test_name);
    retval = 0;
  } else if (mat->capacity != 9) {
    fprintf(stderr, "%s: wrong capacity, expect %d, actual %lu\n", test_name, 9,
            mat->capacity);
  } else {
    retval = 1;
  }

  destr_mat(mat);
  return retval;
}

int
test_mmat_destroy(char* test_name) {
  Mat* mat;
  int retval;

  strcpy(test_name, "Test Mat matrix destruction");
  mat = creat_mat(3, 3);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    return 0;
  }

  destr_mat(mat);
  return retval;
}
int
test_mmat_mul_wrong_size(char* test_name) {
  Mat *mata = creat_mat(2, 3), *matb = creat_mat(2, 4), *matr = creat_mat(2, 4);
  int retval;
  if (!mmat_mul(matr, mata, matb)) {
    fprintf(stderr, "%s: expect mmat_mul to fail but it succeeded\n",
            test_name);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
  return retval;
}

int
test_mmat_add(char* test_name) {
  double mataval[] = {1, 2, 3, 4, 5, 6};
  double matbval[] = {7, 8, 9, 10, 11, 12};
  double expected_sum[] = {8, 10, 12, 14, 16, 18};
  Mat *mata, *matb, *matr;
  int retval;
  size_t i, j;

  strcpy(test_name, "Test Mat addition");
  mata = creat_mat_from_val(mataval, 3, 2);
  matb = creat_mat_from_val(matbval, 3, 2);
  if (mata == NULL || matb == NULL) {
    fprintf(stderr, "%s: expect creating mats successfully but it failed\n",
            test_name);
    return 0;
  }

  matr = creat_mat(3, 2);
  if (matr == NULL) {
    fprintf(stderr,
            "%s: expect creating result matrix successfully but it failed\n",
            test_name);
    destr_mat(mata);
    destr_mat(matb);
    return 0;
  }

  if (mmat_add(matr, mata, matb) != 0) {
    fprintf(
        stderr,
        "%s: expect matrix addition operation to be successful but it failed\n",
        test_name);
    retval = 0;
    goto cleanup;
  }

  // Check if the dimensions of the result matrix are correct
  if (matr->row != 3 || matr->col != 2) {
    fprintf(stderr, "%s: wrong dimensions for result matrix\n", test_name);
    retval = 0;
    goto cleanup;
  }

  for (i = 0; i < matr->row; ++i) {
    for (j = 0; j < matr->col; ++j) {
      if (!double_eq(matr->val[i * matr->col + j],
                     expected_sum[i * matr->col + j])) {
        fprintf(stderr,
                "%s: expect element at (%zu,%zu) to be %lf, actual: %lf\n",
                test_name, i, j, expected_sum[i * matr->col + j],
                matr->val[i * matr->col + j]);
        retval = 0;
        goto cleanup;
      }
    }
  }

  retval = 1;

cleanup:
  destr_mat(mata);
  destr_mat(matb);
  destr_mat(matr);
  return retval;
}

int
test_mmat_transpose(char* test_name) {
  double matval[] = {1, 2, 3, 4, 5, 6, 7, 8};
  double expected_transposed[] = {1, 5, 2, 6, 3, 7, 4, 8};
  Mat* mat;
  int retval;
  size_t i, j;

  strcpy(test_name, "Test Mat transpose");
  mat = creat_mat_from_val(matval, 2, 4);
  if (mat == NULL) {
    fprintf(stderr, "%s: expect creating mat successfully but it failed\n",
            test_name);
    return 0;
  }

  mmat_transpose(mat);

  if (mat->col != 2 || mat->row != 4) {
    fprintf(
        stderr,
        "%s: expect col to be %d, row to be %d. Actual col: %lu, row: %lu\n",
        test_name, 2, 4, mat->col, mat->row);
    destr_mat(mat);
    return 0;
  }

  for (i = 0; i < mat->row; ++i) {
    for (j = 0; j < mat->col; ++j) {
      if (!double_eq(mat->val[i * mat->col + j],
                     expected_transposed[i * mat->col + j])) {
        fprintf(stderr,
                "%s: expect element at (%zu,%zu) to be %lf, actual: %lf\n",
                test_name, i, j, expected_transposed[i * mat->col + j],
                mat->val[i * mat->col + j]);
        destr_mat(mat);
        return 0;
      }
    }
  }

  destr_mat(mat);
  return 1;
}

int
test_mmat_add_same_pointer(char* test_name) {
  double mataval[] = {1, 2, 3, 4, 5, 6};
  double matbval[] = {7, 8, 9, 10, 11, 12};
  double expected_sum[] = {8, 10, 12, 14, 16, 18};
  Mat *mata, *matb;
  int retval;
  size_t i, j;

  strcpy(test_name,
         "Test Mat addition using same pointer for result and input matrix");
  mata = creat_mat_from_val(mataval, 3, 2);
  matb = creat_mat_from_val(matbval, 3, 2);
  if (mata == NULL || matb == NULL) {
    fprintf(stderr, "%s: expect creating mats successfully but it failed\n",
            test_name);
    return 0;
  }

  if (mmat_add(mata, mata, matb) != 0) {
    fprintf(
        stderr,
        "%s: expect matrix addition operation to be successful but it failed\n",
        test_name);
    retval = 0;
    goto cleanup;
  }

  // Check if the dimensions of the result matrix are correct
  if (mata->row != 3 || mata->col != 2) {
    fprintf(stderr, "%s: wrong dimensions for result matrix\n", test_name);
    retval = 0;
    goto cleanup;
  }

  for (i = 0; i < mata->row; ++i) {
    for (j = 0; j < mata->col; ++j) {
      if (!double_eq(mata->val[i * mata->col + j],
                     expected_sum[i * mata->col + j])) {
        fprintf(stderr,
                "%s: expect element at (%zu,%zu) to be %lf, actual: %lf\n",
                test_name, i, j, expected_sum[i * mata->col + j],
                mata->val[i * mata->col + j]);
        retval = 0;
        goto cleanup;
      }
    }
  }

  retval = 1;

cleanup:
  destr_mat(mata);
  destr_mat(matb);
  return retval;
}
