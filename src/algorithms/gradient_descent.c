#include "algorithms/gradient_descent.h"

#include "config.h"
#include "custom_math.h"
#include "data.h"
#include "io.h"
#include "utils.h"

/* Run Stochastic gradient descent algorithm
 *
 * Run 1 epoch (1 pass) through all training examples loader is borrowed from
 * caller
 */
static int sgd(DatLoader *loader);
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

  /* Borrow pointers. All of the following pointers have lifetimes equivalent
   * to grad_desc */
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
    while ((loader = make_data_loader(ld_conf)) && sgd(loader) &&
           epoch++ < max_epoch) {
      if (config->loss_reporter)
        config->loss_reporter(epoch - 1, calc_loss(&loss_result));
      destroy_dat_loader(loader);
    };
  }
  return TRUE;
}

static int
calc_loss(double *result) {
  int i;
  size_t n;
  double loss = 0;
  DatLoader *loader;
  Point buffer[BUFFER_SIZE];
  Point *curr;
  double dot_result;
  int retval = TRUE;

  if ((loader = make_data_loader(loader_conf)) == NULL) {
    rp_err("Calculate loss error, can't make data loader");
    return FALSE; /* No need to cleanup */
  }
  while (!ld_err(loader) && (n = load_data(loader, BUFFER_SIZE, buffer)) > 0) {
    for (i = 0; i < n; ++i) {
      curr = buffer + i;
      dot_product(&dot_result, curr->x, theta, curr->x_length);
      loss += (dot_result - curr->y) * (dot_result - curr->y);
    }
  }
  if (ld_err(loader)) {
    rp_err("calc_loss: error when load data");
    retval = FALSE;
  } else {
    *result = loss / 2;
    retval = TRUE;
  }
  destroy_dat_loader(loader);
  return retval;
}

void
init_point(Point *point, size_t dim) {
  int i;

  point->x_length = dim;
  point->y = 0;
  for (i = 0; i < dim; ++i) {
    point->x[i] = 0;
  }
}

int
sgd(DatLoader *loader) {
  Point point;
  double coeff;
  size_t dim = config->dimension;
  double *temp = (double *)safe_malloc(dim * sizeof(double)); /* owner */
  size_t size = 0;
  int retval = 0;

  init_point(&point, dim);
  while (!(ld_err(loader)) && (size = load_data(loader, 1, &point)) > 0) {
    dot_product(point.x, theta, &coeff, dim);
    coeff = config->learn_rate * (point.y - coeff);
    vec_mul(temp, point.x, coeff, dim);
    vec_add(theta, theta, temp, dim);
  }
  if (ld_err(loader)) {
    rp_err("SGD load data error");
    retval = FALSE;
  } else {
    retval = TRUE;
  }
  safe_free((void **)&(point.x));
  safe_free((void **)&temp);
  return retval;
}

void
init_theta() {
  int i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
