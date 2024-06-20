#include "custom_math.h"

#include <stdio.h>
#include <string.h>

#include "data.h"
#include "utils.h"

double
dot_product(double *a, double *b, size_t dimension) {
  double result = 0;
  while (dimension--) result += a[dimension] * b[dimension];
  return result;
}

void
vec_add(double *result, double *a, double *b, size_t dimension) {
  while (dimension--) result[dimension] = a[dimension] + b[dimension];
}

void
vec_mul(double *result, double *vec, double s, size_t dimension) {
  while (dimension--) result[dimension] = s * vec[dimension];
}

void
mat_mul(double *r, double *a, double *b, size_t s1, size_t s2, size_t s3) {
  size_t i, j, k;
  double temp;

  for (i = 0; i < s1; ++i) {
    for (k = 0; k < s3; ++k) {
      temp = 0;
      for (j = 0; j < s2; ++j) {
        temp += a[i * s2 + j] * b[j * s3 + k];
      }
      r[i * s3 + k] = temp;
    }
  }
}

void
mat_add(double *r, double *a, double *b, size_t s1, size_t s2) {
  size_t i, j;

  for (i = 0; i < s1; ++i) {
    for (j = 0; j < s2; ++j) {
      r[i * s2 + j] = a[i * s2 + j] + b[i * s2 + j];
    }
  }
}

void
mat_transpose(double *mat, size_t s1, size_t s2) {
  size_t i, c, new_c;
  double temp;

  /* Skip this because there would not be any changes */
  if (s1 == 1 || s2 == 1) return;
  for (i = 0; i < s1 * s2; ++i) {
    c = i;
    do {
      c = s2 * (c % s1) + (c / s1);
    } while (c > i);
    if (c < i) continue;
    temp = mat[c];
    while ((new_c = s2 * (c % s1) + (c / s1)) > i) {
      mat[c] = mat[new_c];
      c = new_c;
    }
    mat[c] = temp;
  }
}

/* TODO: Benchmark to check if malloc is costlier or moving values around is
 * costlier. For simplicity, just use malloc */
Mat *
mmat_mul(Mat *a, Mat *b) {
  size_t arr_len;
  double *temp;
  Mat *ret;
  size_t arow, acol;

  if (a == NULL || b == NULL) return NULL;
  if (a->col != b->row) return NULL;
  arow = a->row;
  acol = a->col;
  arr_len = arow * acol;
  temp = (double *)safe_malloc(sizeof(*temp) * arr_len);
  memcpy(temp, a->val, arr_len * sizeof(*temp));
  if (mat_resize(a, a->row, b->col) == NULL) {
    ret = NULL;
    goto cleanup;
  }
  /* After mat_resize is called, a->col is already set to b->row */
  mat_mul(a->val, temp, b->val, arow, acol, b->col);
  ret = a;
cleanup:
  safe_free((void *)&temp);
  return ret;
}

Mat *
mmat_add(Mat *a, Mat *b) {
  if (a == NULL || b == NULL) return NULL;
  if (a->col != b->col || a->row != b->row) {
    return NULL;
  }
  mat_add(a->val, a->val, b->val, a->col, a->row);
  return a;
}

/* TODO:
 * 1. Implement a transpose flag in the Mat struct.
 * 2. Ensure other matrix operations check and respect this transpose flag.
 */
Mat *
mmat_transpose(Mat *mat) {
  double temp;

  if (mat == NULL) return NULL;
  if (mat->val == NULL) return NULL;
  mat_transpose(mat->val, mat->row, mat->col);
  temp = mat->col;
  mat->col = mat->row;
  mat->row = temp;
  return mat;
}

Mat *
mmat_times(Mat *mat, double a) {
  size_t n;

  if (mat == NULL) return NULL;
  n = mat->row * mat->col;
  while (n--) mat->val[n] *= a;
  return mat;
}
