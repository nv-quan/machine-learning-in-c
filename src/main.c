#include <stdio.h>
#include <string.h>

#include "algorithms/gradient_descent.h"
#include "config.h"
#include "data.h"
#include "io.h"

static void init_grad_desc_conf(GDConf *conf);
static void init_loader_conf(DLConf *conf);
static void report_loss(int epoch, double loss);

int
main(void) {
  GDConf grad_desc_conf;
  DLConf data_loader_conf;
  double result[CF_FEAT_DIM];

  init_grad_desc_conf(&grad_desc_conf);
  init_loader_conf(&data_loader_conf);
  grad_desc(&grad_desc_conf, &data_loader_conf, result);
  return 0;
}

static void
init_grad_desc_conf(GDConf *conf) {
  conf->batch_size = CF_BATCH_SZ;
  conf->learn_rate = CF_LRATE;
  conf->dimension = CF_FEAT_DIM;
  conf->loss_reporter = report_loss;
}

static void
report_loss(int epoch, double loss) {
  printf("Epoch %d: %lf\n", epoch, loss);
}

static void
init_loader_conf(DLConf *conf) {
  int i;
  int feature_columns[] = CF_FEAT_COLS;

  conf->is_mem = 0;
  conf->has_header = 1;
  conf->x_dim = CF_FEAT_DIM;
  for (i = 0; i < CF_FEAT_DIM; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = CF_OUTPUT_COL;
  strcpy(conf->file_path, CF_CSV_PATH);
}
