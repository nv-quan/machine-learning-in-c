#ifndef DATA_H
#define DATA_H
#include <stdlib.h>
#define MAX_FEATURE_SIZE 1000

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
  double x[MAX_FEATURE_SIZE];
  double y;
} Point;

#endif
