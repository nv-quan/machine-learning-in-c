#include "config.h"
#include "io.h"
#include <stdio.h>

void stochastic_gradient_descent(const Config *config, DataGetter x_getter,
                                 DataGetter y_getter) {
  double x[BUFFER_SIZE], y[BUFFER_SIZE];
  while (x_getter((void *)x, BUFFER_SIZE) != EOF &&
         y_getter((void *)y, BUFFER_SIZE) != EOF) {
    exit(0);
  }
}
