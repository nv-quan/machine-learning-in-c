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
