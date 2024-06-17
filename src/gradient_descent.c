#include "gradient_descent.h"

#include <stdio.h>

#include "assert.h"
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
static int do_gd(DatLoader *loader);
static void init_theta();

/* Calculate loss function. Return loss on success and -1 on error */
static double calc_loss();

static double *theta;
static Mat *theta_mat, *X_mat, *Y_mat, *temp1;
static GDConf *config;
static DLConf *loader_conf;
static int default_stop_cond(int epoch, double loss);

int
grad_desc(GDConf *gd_conf, DLConf *conf, double *result) {
  int i, retval;
  double loss_result;
  DatLoader *loader;
  int (*stop_cond)(int, double);

  theta = result;
  /* Allocate resources */
  theta_mat = mat_creat_from_val(theta, gd_conf->dimension, 1);
  X_mat = mat_creat(gd_conf->dimension, gd_conf->batch_size);
  Y_mat = mat_creat(1, gd_conf->batch_size);
  temp1 = mat_creat(1, 1);

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

cleanup:
  /*
  theta = NULL;
  config = NULL;
  loader_conf = NULL;
  */
  mat_destr(temp1);
  mat_destr(Y_mat);
  mat_destr(X_mat);
  mat_destr(theta_mat);
  return retval;
}

static int
default_stop_cond(int epoch, double loss) {
  UNUSED(loss);
  return epoch >= 150;
}

static double
calc_loss() {
  size_t i, n;
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

/*
static double
calc_coef(Point *points, size_t len, size_t dim, double alpha) {
  double coef;

  coef = 0;
  while (len--) {
    coef += alpha * (points[len].y - dot_product(theta, points[len].x, dim));
  }
  return coef;
}
*/

void
set_xy_mat(Point *points, size_t size) {
  X_mat = mat_resize(X_mat, config->dimension, size);
  Y_mat = mat_resize(Y_mat, 1, size);
  while (size--) {
    mat_set_col(X_mat, size, points[size].x);
    mat_set_col(Y_mat, size, &points[size].y);
  }
}

int
do_gd(DatLoader *loader) {
  Point points[CF_MAX_BUF_SIZE];
  size_t size, batch_sz, theta_row, theta_col, theta_size;
  Mat *lc_theta_clone, *lc_x, *lc_y, *lc_theta;
  int retval = 0;
  double *theta_val;

  /*
  dim = config->dimension;
  */
  batch_sz = config->batch_size;

  if (batch_sz > sizeof(points)) {
    fprintf(stderr, "Batch size too big, use size <= %lu\n", sizeof(points));
    retval = FALSE;
  }

  theta_row = mat_get_rowc(theta_mat);
  theta_col = mat_get_colc(theta_mat);
  theta_size = theta_row * theta_col;
  theta_val = mat_get_val(theta_mat);
  while (!(ld_err(loader)) &&
         (size = load_data(loader, batch_sz, points)) > 0) {
    set_xy_mat(points, size);
    /* Use local pointers to keep global pointers from being changed */
    lc_x = X_mat;
    lc_y = Y_mat;
    lc_theta = theta_mat;
    lc_theta_clone = temp1;

    /* Chaining assignments to make the final result null if any of them fail */
    lc_theta_clone = mat_resize(lc_theta_clone, theta_row, theta_col);
    lc_theta_clone = mat_set_val(lc_theta_clone, theta_val, theta_size);
    lc_theta_clone = mmat_transpose(lc_theta_clone);
    lc_theta_clone = mmat_mul(lc_theta_clone, lc_x);
    lc_y = mmat_times(lc_y, -1.0);
    lc_theta_clone = mmat_add(lc_theta_clone, lc_y);
    lc_theta_clone = mmat_transpose(lc_theta_clone);
    lc_x = mmat_mul(lc_x, lc_theta_clone);
    lc_x = xmmat_times(lc_x, -1 * config->learn_rate);
    lc_theta_clone = mmat_add(theta_mat, lc_x);

    /* Save points into X & Y */
    /*
    X = make_points_mat_x(points, size);
    Y = make_points_mat_y(points, size);
    */
    /* Temp = mat_creat(1, size); No need for a temp */
    /*
    if (mmat_mul(Theta_trans, X) == NULL) {
      * rp_err("do_gd: Can't do matrix multiplication"); Just return null if
       * the operation is wrong. Any operation with Mat will need to check for
       * null in the arguments and return null if it finds one
      * No need to continuously destroying mat
      mat_destr(Temp);
      mat_destr(X);
      mat_destr(Y);

      return FALSE;
    }
  */
    /* mmat_neg(Y); */
    /*
    if (mmat_add(Temp, Y)) {
      rp_err("do_gd: Can't do matrix multiplication");
      mat_destr(Temp);
      mat_destr(X);
      mat_destr(Y);
      return FALSE;
    }
    */

    /*vec_mul(temp, points.x, coeff, dim);
    vec_add(theta, theta, temp, dim); */
    /*
    mat_destr(Temp);
    mat_destr(X);
    mat_destr(Y);
    */
  }
  if (ld_err(loader)) {
    rp_err("SGD load data error");
    retval = FALSE;
  } else {
    retval = TRUE;
  }
  /*cleanup:*/
  /* mat_destr(Theta); */
  /*mat_destr(Theta_trans); */
  return retval;
}

void
init_theta() {
  size_t i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
