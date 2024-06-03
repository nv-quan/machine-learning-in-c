#include "data.h"

#include <stdio.h>

#include "utils.h"

void
log_point(Point *p) {
  int i;

  printf("[POINT] y: %lf, x: ", p->y);
  for (i = 0; i < p->x_length; ++i) {
    printf("%lf%s", p->x[i], i < p->x_length - 1 ? " \n" : "\n");
  }
}
