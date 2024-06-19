#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void
log_point(Point *p) {
  size_t i;

  printf("[POINT] y: %lf, x: ", p->y);
  for (i = 0; i < p->x_length; ++i) {
    printf("%lf%s", p->x[i], i < p->x_length - 1 ? " " : "\n");
  }
}

void
log_mat(Mat *mat, const char *name) {
  size_t i, j;

  if (mat == NULL) {
    rp_err("Can't log NULL");
    return;
  }
  printf("[MAT %s]\n", name);
  for (i = 0; i < mat->row; ++i) {
    for (j = 0; j < mat->col; ++j) {
      printf("%lf%s", mat_get(mat, i, j), j == mat->col - 1 ? "" : " ");
    }
    printf("\n");
  }
}

Mat *
mat_creat(size_t row, size_t col) {
  if (row * col == 0) {
    return NULL;
  }
  double *val = (double *)safe_malloc(sizeof(*val) * row * col);
  Mat *mat = (Mat *)safe_malloc(sizeof(*mat));
  mat->row = row;
  mat->col = col;
  mat->val = val;
  mat->capacity = row * col;
  return mat;
}

void
mat_destr(Mat *mat) {
  if (mat == NULL) return;
  safe_free((void **)&mat->val);
  safe_free((void **)&mat);
}

Mat *
mat_creat_from_val(double *val, size_t row, size_t col) {
  Mat *res = mat_creat(row, col);
  memcpy((void *)res->val, (const void *)val, sizeof(double) * row * col);
  return res;
}

void
mat_set(Mat *mat, size_t i, size_t j, double val) {
  mat->val[i * mat->col + j] = val;
}

double
mat_get(Mat *mat, size_t i, size_t j) {
  return mat->val[i * mat->col + j];
}

Mat *
mat_set_row(Mat *mat, size_t row_idx, double *value) {
  if (mat == NULL) return NULL;
  double *row = mat->val + mat->col * row_idx;
  memcpy((void *)row, (const void *)value, sizeof(double) * mat->col);
  return mat;
}

Mat *
mat_set_col(Mat *mat, size_t col_idx, double *value) {
  size_t i;
  if (mat == NULL) return NULL;
  for (i = 0; i < mat->row; ++i) {
    mat_set(mat, i, col_idx, value[i]);
  }
  return mat;
}

Mat *
mat_resize(Mat *mat, size_t new_row, size_t new_col) {
  double *temp;
  size_t new_cap;

  if (mat == NULL) return NULL;
  new_cap = new_row * new_col;
  if (mat->capacity < new_cap) {
    temp = (double *)realloc(mat->val, new_cap * sizeof(*temp));
    if (temp == NULL) return NULL;
    mat->capacity = new_cap;
    mat->val = temp;
  }
  mat->row = new_row;
  mat->col = new_col;
  return mat;
}

Mat *
mat_set_val(Mat *mat, double *arr, size_t len) {
  if (mat == NULL) return NULL;
  if (len != mat->row * mat->col) return NULL;
  if (arr == NULL) return NULL;
  memcpy(mat->val, arr, len * sizeof(*arr));
  return mat;
}

Mat *
mat_clone(Mat *mat) {
  return mat_creat_from_val(mat->val, mat->row, mat->col);
}

size_t
mat_get_rowc(Mat *mat) {
  if (mat == NULL) return 0;
  return mat->row;
}

size_t
mat_get_colc(Mat *mat) {
  if (mat == NULL) return 0;
  return mat->col;
}

double *
mat_get_val(Mat *mat) {
  if (mat == NULL) return NULL;
  return mat->val;
}
