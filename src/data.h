#ifndef DATA_H
#define DATA_H
#include <stdlib.h>

typedef enum data_type {
  INT,
  LONG,
  FLOAT,
  DOUBLE
} DataType;

typedef union number {
  int _int;
  long int _long;
  float _float;
  double _double;
} Number;

typedef struct data_tag {
  DataType type;
  Number value;
} Data;

typedef struct data_list_tag {
  Number *content;
  size_t length;
}

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
