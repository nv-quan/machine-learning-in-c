#include "algorithms/gradient_descent.h"
#include "config.h"
#include "custom_math.h"
#include "data.h"
#include "io.h"
#include "utils.h"

#define MAX_BUFFER_SIZE 1024

/* Run Stochastic gradient descent algorithm
 *
 * Run 1 epoch (1 pass) through all training examples loader is borrowed from
 * caller
 */
static int sgd(DatLoader *loader);
static int getdata(DataGetter getter, double *buffer);
static void init_theta();

/* Calculate loss function
 *
 * Save result into [result]
 *
 * Return 0 on error, and 1 on success
 */
static int calc_loss(double *result);

static double *theta;
static GDConf *config;
static DLConf *loader_conf;

/* TODO: Refactor to put ld_conf to gd_conf because x_dim and dimension overlaps
 */
int grad_desc(GDConf *gd_conf, DLConf *ld_conf, double *result) {
  int epoch = 0;
  int max_epoch = 150;
  int current_loss;
  double loss_result;
  DatLoader *dat_loader;

  /* Borrow pointers. All of the following pointers have lifetimes equivalent to
   * grad_desc */
  theta = result;
  config = gd_conf;
  loader_conf = ld_conf;

  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    return FALSE;
  }
  if (config->batch_size == 1) {
    /* Stochastic gradient descent */
    while ((dat_loader = make_data_loader(loader_conf)) != 0 &&
           sgd(dat_loader) != 0 && epoch++ < max_epoch) {
      if (config->loss_reporter)
        config->loss_reporter(epoch - 1, calc_loss(&loss_result));
      destroy_dat_loader(dat_loader);
    };
  }
  return TRUE;
}

static int getdata(DataGetter getter, double *buffer) {
  return getter((void *)buffer, BUFFER_SIZE * sizeof(double));
}

static int calc_loss(double *result) {
  int i;
  size_t dat_size;
  double loss = 0;
  DatLoader *dat_loader;
  Point buffer[MAX_BUFFER_SIZE];
  Point *curr;
  double dot_result;
  int retval = TRUE;

  if ((dat_loader = make_data_loader(loader_conf)) == NULL) {
    rp_err("Calculate loss error, can't make data loader");
    return FALSE; /* No need to cleanup */
  }
  while (!ld_err(dat_loader) &&
         (dat_size = load_data(dat_loader, MAX_BUFFER_SIZE, buffer)) > 0) {
    for (i = 0; i < dat_size; ++i) {
      curr = buffer + i; /* Borrow */
      if (dot_product(&dot_result, curr->x, theta, curr->x_length) == 0) {
        rp_err("SGD error, can't do dot product");
        retval = FALSE;
        goto cleanup;
      }
      loss += (dot_result - curr->y) * (dot_result - curr->y);
    }
  }
  if (ld_err(dat_loader)) {
    rp_err("calc_loss: error when load data");
    retval = FALSE;
    goto cleanup;
  }
  *result = loss / 2;
  retval = TRUE;

cleanup:
  destroy_dat_loader(dat_loader);
  return retval;
}

int sgd(DatLoader *loader) {
  Point point;
  double coeff;
  size_t dim = config->dimension;
  double *temp = (double *)safe_malloc(dim * sizeof(double)); /* owner */
  size_t size = 0;
  int retval = 0;

  point.x_length = dim;
  point.x = (double *)safe_malloc(dim * sizeof(double));
  while (!(ld_err(loader)) && (size = load_data(loader, 1, &point)) > 0) {
    if (dot_product(point.x, theta, &coeff, dim) == 0) {
      rp_err("SGD error, can't do dot product");
      retval = FALSE;
      goto cleanup;
    }
    coeff = config->learn_rate * (point.y - coeff);
    if (vec_mul(temp, point.x, coeff, dim) == 0) {
      rp_err("SGD error, can't do vector multiplication");
      retval = FALSE;
      goto cleanup;
    }
    vec_add(theta, theta, temp, dim);
  }
  if (ld_err(loader)) {
    rp_err("SGD load data error");
    retval = FALSE;
    goto cleanup;
  }
  retval = TRUE;

cleanup:
  safe_free((void **)&(point.x));
  safe_free((void **)&temp);
  return retval;
}

void init_theta() {
  int i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
