#include <stdlib.h>
#include "data.h"
#include "utils.h"

Data makeData(double x, double y)
{
    Data temp;
    temp.x = x;
    temp.y = y;
    return temp;
}

DoubleArray make_double_array(size_t size) {
  DoubleArray new_array;
  double *array_content = (double *) safe_malloc(sizeof(double) * size);
  new_array.content = array_content;
}
