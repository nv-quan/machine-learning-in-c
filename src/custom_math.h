#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H
#include <stdlib.h>

/* Calculate dot product of 2 vector a and b */
double dot_product(double *a, double *b, size_t dimension);

/* Add 2 vector a & b */
void vec_add(double *result, double *a, double *b, size_t dimension);

/* Multiply a vector with a scalar */
void vec_mul(double *result, double *vec, double s, size_t dimension);

/* Matrix multiplication. r is the result of a x b.
 * Here, the matrices are flatten into continuous memory blocks by concatenating
 * their rows together.
 *
 * a's size is s1 x s2, b's size is s2 x s3, r's size is s1 x s3 */
void mat_mul(double *r, double *a, double *b, size_t s1, size_t s2, size_t s3);

/* Matrix addition. r is the result of a + b.
 * Matrices are flatten into continuous memory blocks by concatenating their
 * rows together.
 *
 * Sizes of r, a, b are s1 x s2 */
void mat_add(double *r, double *a, double *b, size_t s1, size_t s2);

/* Inplace transpose a s1 x s2 matrix to s2 x s1 matrix */
void mat_transpose(double *mat, size_t s1, size_t s2);

#endif /* ifndef CUSTOM_MATH_H */
