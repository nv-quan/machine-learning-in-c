#include "custom_math.h"

void
dot_product(double *result, double *a, double *b, size_t dimension) {
  while (dimension--) result[dimension] = a[dimension] * b[dimension];
}

void
vec_add(double *result, double *a, double *b, size_t dimension) {
  while (dimension--) result[dimension] = a[dimension] + b[dimension];
}

void
vec_mul(double *result, double *vec, double s, size_t dimension) {
  while (dimension--) result[dimension] = s * vec[dimension];
}
