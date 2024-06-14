#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H
#include <stdlib.h>

#include "data.h"

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

/* Matrix multiplication of a and b. The result is saved into a.
 * Return the pointer to a on success and NULL pointer on failure */
Mat *mmat_mul(Mat *a, Mat *b);

/* Matrix addition of a and b. The result is saved into a.
 * Return the pointer to a on success and NULL pointer on failure */
Mat *mmat_add(Mat *a, Mat *b);

/* Matrix transpose using Mat. Modify mat and return the same pointer on
 * success. On failure, keep mat unchanged and return null */
Mat *mmat_transpose(Mat *mat);

/* Multiply a matrix and a scalar.
 * Return the pointer to mat or NULL on failure */
Mat *mmat_times(Mat *mat, double a);

#endif /* ifndef CUSTOM_MATH_H */
