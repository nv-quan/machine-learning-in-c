#include "algorithms/gradient_descent.h"
#include "config.h"
#include "custom_math.h"
#include "data.h"
#include "io.h"
#include "utils.h"

/* Run Stochastic gradient descent algorithm */
static int sgd();
static int getdata(DataGetter getter, double *buffer);
static void init_theta();
/* Calculate loss function */
static double calc_loss();

static double *theta;
static GDConf *config;
static DatLoader *loader;

int grad_desc(GDConf *gd_conf, DatLoader *dat_loader, double *result) {
  int epoch = 0;
  int max_epoch = 150;
  int current_loss;
  theta = result;      /* borrow */
  config = gd_conf;    /* borrow */
  loader = dat_loader; /* borrow */
  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    return FALSE;
  }
  if (config->batch_size == 1) {
    /* Stochastic gradient descent */
    while (sgd() != 0 && epoch++ < max_epoch) {
      config->loss_reporter(epoch - 1, calc_loss());
    };
  }
  return TRUE;
}

static int getdata(DataGetter getter, double *buffer) {
  return getter((void *)buffer, BUFFER_SIZE * sizeof(double));
}

static double calc_loss() { return 0L; }

int sgd() {
  Point point;
  double coeff;
  size_t dim = config->dimension;
  double *temp = (double *)safe_malloc(dim * sizeof(double)); /* owner */

  point.x_length = dim;
  point.x = (double *)safe_malloc(dim * sizeof(double));
  while (load_data(loader, 1, &point) != 0) {
    if (dot_product(point.x, theta, &coeff, dim) == 0) {
      rp_err("SGD error, can't do dot product");
      return FALSE;
    }
    coeff = config->learn_rate * (point.y - coeff);
    if (vec_mul(temp, point.x, coeff, dim) == 0) {
      rp_err("SGD error, can't do vector multiplication");
      return FALSE;
    }
    vec_add(theta, theta, temp, dim);
  }
  safe_free((void **)&(point.x));
  safe_free((void **)&temp);
  return TRUE;
}

void init_theta() {
  int i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
