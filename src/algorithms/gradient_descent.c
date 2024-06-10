#include "algorithms/gradient_descent.h"

#include <stdio.h>

#include "config.h"
#include "custom_math.h"
#include "data.h"
/* #include "debug.h" */
#include "assert.h"
#include "io.h"
#include "utils.h"

/* Run Stochastic gradient descent algorithm
 *
 * Run 1 epoch (1 pass) through all training examples loader is borrowed from
 * caller
 */
static int do_gd(DatLoader *loader);
static void init_theta();

/* Calculate loss function. Return loss on success and -1 on error */
static double calc_loss();

static double *theta;
static GDConf *config;
static DLConf *loader_conf;
static int default_stop_cond(int epoch, double loss);

int
grad_desc(GDConf *gd_conf, DLConf *conf, double *result) {
  int i, current_loss, retval;
  double loss_result;
  DatLoader *loader;
  int (*stop_cond)(int, double);

  /* Borrow pointers. All of the following pointers have lifetimes equivalent
   * to grad_desc */
  theta = result;
  config = gd_conf;
  loader_conf = conf;

  init_theta();
  if (config->batch_size == 0) {
    rp_err("Gradient descent error: 0 batch size");
    retval = FALSE;
    goto cleanup;
  }
  i = 0;
  if (config->stop_cond)
    stop_cond = config->stop_cond;
  else
    stop_cond = default_stop_cond;
  while ((loader = make_data_loader(conf)) && do_gd(loader)) {
    i++;
    loss_result = calc_loss();
    if (config->loss_reporter) config->loss_reporter(i - 1, loss_result);
    destroy_dat_loader(loader);
    if (stop_cond(i, loss_result)) break;
  };
  retval = TRUE;

  /* Prevent accidentally accessing these variables after grad_desc finishes */
cleanup:
  theta = NULL;
  config = NULL;
  loader_conf = NULL;
  return retval;
}

static int
default_stop_cond(int epoch, double loss) {
  return epoch >= 150;
}

static double
calc_loss() {
  int i;
  size_t n;
  double dot_result, loss = 0;
  DatLoader *loader;
  Point buffer[BUFFER_SIZE], *curr;

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

static double
calc_coef(Point *points, size_t len, size_t dim, double alpha) {
  double coef;

  while (len--) {
    coef += alpha * (points[len].y - dot_product(theta, points[len].x, dim));
  }
  return coef;
}

Mat *
make_points_mat_x(Point *points, size_t size) {
  size_t i;
  Mat *points_mat = creat_mat(points[0].x_length, size);
  for (i = 0; i < size; ++i) {
    set_mat_col(points_mat, i, points[i].x);
  }
  return points_mat;
}

Mat *
make_points_mat_y(Point *points, size_t size) {
  size_t i;
  Mat *y = creat_mat(1, size);
  for (i = 0; i < size; ++i) {
    set_mat_col(y, i, &points[i].y);
  }
  return y;
}

int
do_gd(DatLoader *loader) {
  Point points[CF_MAX_BUF_SIZE];
  double coeff;
  size_t size, dim, batch_sz;
  Mat *X, *Y, *Theta, *Theta_trans, *Temp;
  int retval = 0;

#ifdef DEBUG_H
  print_arr("theta entering sgd", theta, config->dimension);
#endif
  dim = config->dimension;
  batch_sz = config->batch_size;

  if (batch_sz > sizeof(points)) {
    fprintf(stderr, "Batch size too big, use size <= %lu\n", sizeof(points));
    retval = FALSE;
  }
  Theta = creat_mat_from_val(theta, dim, 1);
  Theta_trans = creat_mat_from_val(theta, dim, 1);
  mmat_transpose(Theta_trans);
  while (!(ld_err(loader)) &&
         (size = load_data(loader, batch_sz, points)) > 0) {
    X = make_points_mat_x(points, size);
    Y = make_points_mat_y(points, size);
    Temp = creat_mat(1, size);
    if (mmat_mul(Temp, Theta_trans, X)) {
      rp_err("do_gd: Can't do matrix multiplication");
      destr_mat(Temp);
      destr_mat(X);
      destr_mat(Y);
      return FALSE;
    }
    mmat_neg(Y);
    if (mmat_add(Temp, Temp, Y)) {
      rp_err("do_gd: Can't do matrix multiplication");
      destr_mat(Temp);
      destr_mat(X);
      destr_mat(Y);
      return FALSE;
    }

    /*vec_mul(temp, points.x, coeff, dim);
    vec_add(theta, theta, temp, dim); */
#ifdef DEBUG_H
    printf("coeff: %lf\n", coeff);
    print_arr("theta after add", theta, config->dimension);
#endif
    destr_mat(Temp);
    destr_mat(X);
    destr_mat(Y);
  }
  if (ld_err(loader)) {
    rp_err("SGD load data error");
    retval = FALSE;
  } else {
    retval = TRUE;
  }
cleanup:
  destr_mat(Theta);
  destr_mat(Theta_trans);
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
  print_arr("initialized theta", theta, config->dimension + 1);
#endif
}
