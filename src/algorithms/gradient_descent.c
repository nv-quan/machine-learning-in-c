#include "algorithms/gradient_descent.h"
#include "config.h"
#include "custom_math.h"
#include "data.h"
#include "io.h"
#include "utils.h"

#define MAX_BUFFER_SIZE 1024

/* Run Stochastic gradient descent algorithm
 *
 * Run 1 epoch (1 pass) through all training examples
 * loader is borrowed from caller
 */
static int sgd(struct data_loader *loader);
static int getdata(DataGetter getter, double *buffer);
static void init_theta();

/* Calculate loss function
 *
 * Save result into [result]
 *
 * Return
 *  0 on error
 *  1 on success
 */
static int calc_loss(double *result);

static double *theta;
static GDConf *config;
static struct data_loader_config *loader_conf;

int grad_desc(GDConf *gd_conf, struct data_loader_config *ld_conf,
              double *result) {
  int epoch = 0;
  int max_epoch = 150;
  int current_loss;
  double loss_result;
  struct data_loader *dat_loader;

  theta = result;        /* borrow */
  config = gd_conf;      /* borrow */
  loader_conf = ld_conf; /* borrow */
  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    return FALSE;
  }
  if (config->batch_size == 1) {
    /* Stochastic gradient descent */
    dat_loader = (struct data_loader *)safe_malloc(sizeof(struct data_loader));
    while (make_data_loader(loader_conf, dat_loader) == 0 &&
           sgd(dat_loader) != 0 && epoch++ < max_epoch) {
      if (config->loss_reporter)
        config->loss_reporter(epoch - 1, calc_loss(&loss_result));
      clear_data_loader(dat_loader);
    };
    safe_free((void **)&dat_loader);
  }
  return TRUE;
}

static int getdata(DataGetter getter, double *buffer) {
  return getter((void *)buffer, BUFFER_SIZE * sizeof(double));
}

static int calc_loss(double *result) {
  int i;
  int dat_size;
  double loss = 0;
  struct data_loader *dat_loader;
  Point buffer[MAX_BUFFER_SIZE];
  Point *curr;
  double dot_result;
  int retval = TRUE;

  dat_loader = (struct data_loader *)safe_malloc(sizeof(struct data_loader));
  if (make_data_loader(loader_conf, dat_loader) != 0) {
    rp_err("Calculate loss error, can't make data loader");
    retval = FALSE;
    goto cleanup1;
  }
  while ((dat_size = load_data(dat_loader, MAX_BUFFER_SIZE, buffer)) > 0) {
    for (i = 0; i < dat_size; ++i) {
      curr = buffer + i; /* Borrow */
      if (dot_product(&dot_result, curr->x, theta, curr->x_length) == 0) {
        rp_err("SGD error, can't do dot product");
        retval = FALSE;
        goto cleanup2;
      }
      loss += (dot_result - curr->y) * (dot_result - curr->y);
    }
  }
  if (dat_size < 0) {
    if (dat_loader->err_msg[0] != '\0')
      rp_err(dat_loader->err_msg);
    retval = FALSE;
    goto cleanup2;
  }
  *result = loss / 2;
  retval = TRUE;

cleanup2:
  clear_data_loader(dat_loader);
cleanup1:
  safe_free((void **)&dat_loader);
  return retval;
}

int sgd(struct data_loader *loader) {
  Point point;
  double coeff;
  size_t dim = config->dimension;
  double *temp = (double *)safe_malloc(dim * sizeof(double)); /* owner */
  int size = 0;
  int retval = 0;

  point.x_length = dim;
  point.x = (double *)safe_malloc(dim * sizeof(double));
  while ((size = load_data(loader, 1, &point)) > 0) {
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
  if (size < 0) {
    rp_err("SGD: can't load data");
    if (loader->err_msg[0] != '\0')
      rp_err(loader->err_msg);
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
