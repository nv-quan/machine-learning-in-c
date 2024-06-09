#ifndef TEST_MATH_H
#define TEST_MATH_H

#include <custom_math.h>

int test_dot_product(char* name);
int test_vec_add(char* name);
int test_vec_mul(char* name);
int test_double_eq(char* name);
int test_mat_mul(char* name);
int test_mat_add(char* name);
int test_mat_transpose1x8(char* name);
int test_mat_transpose3x5(char* name);
int test_mat_transpose1x1(char* name);
int test_mmat_create(char* test_name);
int test_mmat_create_fromval(char* test_name);
int test_mmat_set_and_get_elem(char* test_name);
int test_mmat_set_row(char* test_name);
int test_mmat_set_col(char* test_name);
int test_mmat_mul(char* test_name);
int test_mmat_mul_wrong_size(char* test_name);
int test_mmat_add(char* test_name);
int test_mmat_add_same_pointer(char* test_name);
int test_mmat_transpose(char* test_name);
int test_mmat_destroy(char* test_name);

#endif /* ifndef TEST_MATH_H */
