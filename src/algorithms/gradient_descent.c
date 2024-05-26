#include "algorithms/gradient_descent.h"
#include "config.h"
#include "data.h"
#include "io.h"
#include "utils.h"
#include "custom_math.h"

/* Run Stochastic gradient descent algorithm */
void sgd();
static int getdata(DataGetter getter, double *buffer);
static void init_theta();

static double *theta;
static GDConf *config;
static DatLoader *loader;

int grad_desc(GDConf *gd_conf, DatLoader *dat_loader, double *result) {
  theta = result;   /* borrow */
  config = gd_conf; /* borrow */
  loader = dat_loader; /* borrow */
  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    return FALSE;
  }
  if (config->batch_size == 1) {
    /* Stochastic gradient descent */
  }
  return TRUE;
}

static int getdata(DataGetter getter, double *buffer) {
  return getter((void *)buffer, BUFFER_SIZE * sizeof(double));
}

void sgd() {
  Point point;
  double *result = (double*) safe_malloc(config->dimension * sizeof(double));
  point.x_length = config->dimension;
  point.x = (double*) safe_malloc(point.x_length * sizeof(double));
  while (load_data(loader, 1, &point) != 0) {
    if (dot_product(point.x, theta, config->dimension, result) != 0) {
    }
  }
  safe_free((void**)&(point.x));
  safe_free((void**)&result);
}

void init_theta() {
  int i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
