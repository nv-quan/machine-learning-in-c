#include "algorithms/gradient_descent.h"

#include <stdio.h>

#include "config.h"
#include "custom_math.h"
#include "data.h"
#include "debug.h"
#include "io.h"
#include "utils.h"

/* Run Stochastic gradient descent algorithm
 *
 * Run 1 epoch (1 pass) through all training examples loader is borrowed from
 * caller
 */
static int sgd(DatLoader *loader);
static void init_theta();

/* Calculate loss function. Return loss on success and -1 on error */
static double calc_loss();

static double *theta;
static GDConf *config;
static DLConf *loader_conf;

/* TODO: Refactor to put ld_conf to gd_conf because x_dim and dimension
 * overlaps
 */
int
grad_desc(GDConf *gd_conf, DLConf *ld_conf, double *result) {
  int epoch = 0;
  int max_epoch = 150;
  int current_loss;
  double loss_result;
  DatLoader *loader;
  int retval;

  /* Borrow pointers. All of the following pointers have lifetimes equivalent
   * to grad_desc */
  theta = result;
  config = gd_conf;
  loader_conf = ld_conf;

  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    retval = FALSE;
    goto cleanup;
  }
  if (config->batch_size == 1) {
    /* Stochastic gradient descent */
    while ((loader = make_data_loader(ld_conf)) && sgd(loader) &&
           epoch++ < max_epoch) {
      if (config->loss_reporter) {
        config->loss_reporter(epoch - 1, calc_loss());
      }
      destroy_dat_loader(loader);
    };
  }
  retval = TRUE;

  /* Prevent accidentally accessing these variables after grad_desc finishes */
cleanup:
  theta = NULL;
  config = NULL;
  loader_conf = NULL;
  return retval;
}

static double
calc_loss() {
  int i;
  size_t n;
  double loss = 0;
  DatLoader *loader;
  Point buffer[BUFFER_SIZE];
  Point *curr;
  double dot_result;

  if ((loader = make_data_loader(loader_conf)) == NULL) {
    rp_err("Calculate loss error, can't make data loader");
    return -1; /* No need to cleanup */
  }
  while (!ld_err(loader) && (n = load_data(loader, BUFFER_SIZE, buffer)) > 0) {
    for (i = 0; i < n; ++i) {
      curr = buffer + i;
      dot_result = dot_product(curr->x, theta, curr->x_length);
#ifdef DEBUG_H
      printf("dot_result: %lf, y: %lf\n", dot_result, curr->y);
      print_arr("theta", theta, curr->x_length);
      print_arr("x", curr->x, curr->x_length);
#endif
      loss += (dot_result - curr->y) * (dot_result - curr->y);
    }
  }
  if (ld_err(loader)) {
    rp_err("calc_loss: error when load data");
    loss = -1;
  } else {
    loss /= 2;
  }
  destroy_dat_loader(loader);
  return loss;
}

int
sgd(DatLoader *loader) {
  Point point;
  double coeff;
  size_t dim = config->dimension;
  double temp[CF_FEAT_DIM];
  size_t size = 0;
  int retval = 0;

#ifdef DEBUG_H
  print_arr("theta entering sgd", theta, config->dimension);
#endif
  while (!(ld_err(loader)) && (size = load_data(loader, 1, &point)) > 0) {
    coeff = dot_product(point.x, theta, dim);
    coeff = config->learn_rate * (point.y - coeff);
    vec_mul(temp, point.x, coeff, dim);
    vec_add(theta, theta, temp, dim);
#ifdef DEBUG_H
    print_arr("theta after add", theta, config->dimension);
#endif
  }
  if (ld_err(loader)) {
    rp_err("SGD load data error");
    retval = FALSE;
  } else {
    retval = TRUE;
  }
  return retval;
}

void
init_theta() {
  int i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
#ifdef DEBUG_H
  print_arr("initialized theta", theta, config->dimension);
#endif
}
