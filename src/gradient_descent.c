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
  Point points[BUFFER_SIZE], *curr;

  if ((loader = make_data_loader(loader_conf)) == NULL) {
    rp_err("Calculate loss error, can't make data loader");
    return -1; /* No need to cleanup */
  }
  while (!ld_err(loader) && (n = load_data(loader, BUFFER_SIZE, points)) > 0) {
    for (i = 0; i < n; ++i) {
      curr = points + i;
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
  size_t sz, batch_sz, theta_row, theta_col, theta_size, lc_theta_col,
      lc_theta_row;
  Mat *lc_theta_clone, *lc_x, *lc_y, *lc_theta;
  double *theta_val, *lc_theta_val;

  batch_sz = config->batch_size;

  if (batch_sz > sizeof(points)) {
    fprintf(stderr, "Batch size too big, use size <= %lu\n", sizeof(points));
    return FALSE;
  }

  theta_row = mat_get_rowc(theta_mat);
  theta_col = mat_get_colc(theta_mat);
  theta_size = theta_row * theta_col;
  theta_val = mat_get_val(theta_mat);
  if (X_mat == NULL || Y_mat == NULL || theta_mat == NULL || temp1 == NULL) {
    rp_err("do_gd: Can't read allocated memory.");
    return FALSE;
  }
  if (theta_row == 0 || theta_col == 0) {
    rp_err("do_gd: Theta's dimension is invalid.");
    return FALSE;
  }
  while (!(ld_err(loader)) && (sz = load_data(loader, batch_sz, points)) > 0) {
    set_xy_mat(points, sz);
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
    lc_x = mmat_times(lc_x, -1 * config->learn_rate);
    lc_theta_clone = mmat_add(lc_theta, lc_x);

    /* If some error happens */
    if (lc_theta_clone == NULL) {
      rp_err("do_gd: Some errors happened in matrix operations.");
      return FALSE;
    }
    lc_theta_col = mat_get_colc(lc_theta_clone);
    lc_theta_row = mat_get_rowc(lc_theta_clone);
    lc_theta_val = mat_get_val(lc_theta_clone);
    if (lc_theta_col != theta_col || lc_theta_row != theta_row) {
      rp_err("do_gd: Invalid result.");
      return FALSE;
    }
    if (!mat_set_val(theta_mat, lc_theta_val, lc_theta_row * lc_theta_col)) {
      rp_err("do_gd: Can't set theta value");
      return FALSE;
    }
  }
  if (ld_err(loader)) {
    rp_err("do_gd: SGD load data error.");
    return FALSE;
  }
  return TRUE;
}

void
init_theta() {
  size_t i;
  /* Zero initialization, will revisit later */
  for (i = 0; i < config->dimension; i++) {
    theta[i] = 0;
  }
}
