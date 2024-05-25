#include "algorithms/gradient_descent.h"
#include "config.h"
#include "data.h"
#include "io.h"
#include "utils.h"
#include <stdio.h>

static int getdata(DataGetter getter, double *buffer);
static void init_theta(double *result, size_t dimension);

static double *result;

int grad_desc(GDConfig *config, double *result) {
  init_theta(result, config->dimension);
  switch (config->type) {
  case STOCHASTIC:
    break;
  default:
    break;
  }
  return TRUE;
}

static int getdata(DataGetter getter, double *buffer) {
  return getter((void *)buffer, BUFFER_SIZE * sizeof(double));
}

void sgd(PointGetter point_getter, double *result) {
  Point p;
  while (point_getter(&p) != 0) {

    safe_free((void **)&p.x);
  }
}

void init_theta(double *result, size_t dimension) {
  int i;

  /* Zero initialization, will revisit later */
  for (i = 0; i < dimension; i++) {
    result[i] = 0;
  }
}
