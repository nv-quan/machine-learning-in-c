#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H
#include <stdlib.h>

/* Calculate dot product of 2 vector a and b */
void dot_product(double *result, double *a, double *b, size_t dimension);

/* Add 2 vector a & b */
void vec_add(double *result, double *a, double *b, size_t dimension);

/* Multiply a vector with a scalar */
void vec_mul(double *result, double *vec, double s, size_t dimension);

#endif /* ifndef CUSTOM_MATH_H */
