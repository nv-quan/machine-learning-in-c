#include "custom_math.h"

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

/* Naive implementation of matrix multiplication */
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
