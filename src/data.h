#ifndef DATA_H
#define DATA_H
#include <stdlib.h>

typedef struct data_tag {
  double x;
  double y;
} Data;

typedef struct double_array_tag {
  double *content;
  size_t length;
} DoubleArray;

typedef struct double_2d_array_tag {
  double **content;
  size_t width;
  size_t height;
} Double2DArray;

#endif
