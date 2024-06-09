#include "custom_math.h"

#include <arm_neon.h>

#include "data.h"

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

int
mmat_mul(Mat *r, Mat *a, Mat *b) {
  if (r->row != a->row || r->col != b->col || a->col != b->row) {
    return -1;
  }
  mat_mul(r->val, a->val, b->val, a->row, a->col, b->col);
  return 0;
}

int
mmat_add(Mat *r, Mat *a, Mat *b) {
  if (a->col != b->col || a->row != b->row || a->col != r->col ||
      a->row != r->row) {
    return -1;
  }
  mat_add(r->val, a->val, b->val, a->col, b->col);
  return 0;
}

void
mmat_transpose(Mat *mat) {
  mat_transpose(mat->val, mat->row, mat->col);
}
