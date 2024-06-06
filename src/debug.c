#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void
print_arr(const char* prefix, double* arr, size_t dim) {
  int i;

  printf("%s: ", prefix);
  for (i = 0; i < dim; ++i) {
    printf("%lf%s", arr[i], i == dim - 1 ? "" : " ");
  }
  putchar('\n');
}
