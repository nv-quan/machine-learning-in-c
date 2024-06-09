#include "data.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void
log_point(Point *p) {
  int i;

  printf("[POINT] y: %lf, x: ", p->y);
  for (i = 0; i < p->x_length; ++i) {
    printf("%lf%s", p->x[i], i < p->x_length - 1 ? " " : "\n");
  }
}

Mat *
creat_mat(size_t row, size_t col) {
  if (row * col == 0) {
    return NULL;
  }
  double *val = (double *)safe_malloc(sizeof(*val) * row * col);
  Mat *mat = (Mat *)safe_malloc(sizeof(*mat));
  mat->row = row;
  mat->col = col;
  mat->val = val;
  return mat;
}

void
destr_mat(Mat *mat) {
  safe_free((void **)&mat->val);
  safe_free((void **)&mat);
}

Mat *
creat_mat_from_x(Point *points, size_t len) {
  Mat *result;
  if (len == 0) {
    return NULL;
  }
  result = creat_mat(points[0].x_length, len);
}
