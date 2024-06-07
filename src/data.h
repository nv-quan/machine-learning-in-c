#ifndef DATA_H
#define DATA_H
#include <stdlib.h>

#include "config.h"

/* typedef enum number_type { INT, LONG, FLOAT, DOUBLE };

typedef union number {
  int _int;
  long int _long;
  float _float;
  double _double;
};

typedef struct number_tag {
  enum data_type type;
  union number value;
} DNumber;
*/

/* typedef struct number_array_tag {
  DNumber *content;
  size_t length;
} DNumberArray;

DType d_num_type(const DNumber *number);

 returns -1 on error
DType d_numarr_type(const DNumberArray *numarr);
*/

/* (x, y) data point where x is array of n length and y is double */
typedef struct point_data {
  size_t x_length;
  double x[CF_MAX_DIM];
  double y;
} Point;

void log_point(Point* point);

#endif
