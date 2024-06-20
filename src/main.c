#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "data.h"
#include "gradient_descent.h"
#include "io.h"
#include "utils.h"

static void init_grad_desc_conf(GDConf *conf);
static void init_loader_conf(DLConf *conf);
static void report_loss(int epoch, double loss);

static volatile int is_interrupted = 0;

void
intHandler(int dummy) {
  UNUSED(dummy);
  is_interrupted = 1;
}

int
main(void) {
  GDConf grad_desc_conf;
  DLConf data_loader_conf;
  double result[CF_MAX_DIM];
  size_t i;

  signal(SIGINT, intHandler);
  init_grad_desc_conf(&grad_desc_conf);
  init_loader_conf(&data_loader_conf);
  save_dlconf(&data_loader_conf, "data/loader_conf.txt");
  grad_desc(&grad_desc_conf, &data_loader_conf, result);
  printf("Result:");
  for (i = 0; i < grad_desc_conf.dimension; ++i) {
    printf("%lf%s", result[i], i == grad_desc_conf.dimension - 1 ? "\n" : " ");
  }
  return 0;
}

int
stop_cond(int epoch, double loss) {
  UNUSED(epoch);
  UNUSED(loss);
  return is_interrupted;
}

static void
init_grad_desc_conf(GDConf *conf) {
  conf->batch_size = 414;
  conf->learn_rate = 0.00000000001;
  conf->dimension = 4;
  conf->loss_reporter = report_loss;
  conf->stop_cond = stop_cond;
}

static void
report_loss(int epoch, double loss) {
  if (epoch % 1000 == 0) {
    printf("Epoch %d: %lf\n", epoch, loss);
  }
}

static void
init_loader_conf(DLConf *conf) {
  conf->options = DL_INSERT_ONE | DL_HAS_HEADER;
  conf->x_dim = 4;
  conf->y_col = 7;
  conf->x_cols[1] = 2;
  conf->x_cols[2] = 3;
  conf->x_cols[3] = 4;
  strcpy(conf->file_path, "tests/test_data.csv");
}
