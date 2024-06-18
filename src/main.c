#include <stdio.h>
#include <string.h>

#include "config.h"
#include "data.h"
#include "gradient_descent.h"
#include "io.h"

static void init_grad_desc_conf(GDConf *conf);
static void init_loader_conf(DLConf *conf);
static void report_loss(int epoch, double loss);

int
main(void) {
  GDConf grad_desc_conf;
  DLConf data_loader_conf;
  double result[CF_MAX_DIM];

  init_grad_desc_conf(&grad_desc_conf);
  init_loader_conf(&data_loader_conf);
  grad_desc(&grad_desc_conf, &data_loader_conf, result);
  printf("Result: %lf, %lf\n", result[0], result[1]);
  return 0;
}

static void
init_grad_desc_conf(GDConf *conf) {
  conf->batch_size = 1;
  conf->learn_rate = 0.01;
  conf->dimension = 2;
  conf->loss_reporter = report_loss;
  conf->stop_cond = NULL;
}

static void
report_loss(int epoch, double loss) {
  printf("Epoch %d: %lf\n", epoch, loss);
}

static char data[] =
    "-1.605793084,0.3\n"
    "-1.436762233, 0.2\n"
    "-1.267731382, 0.24\n"
    "-1.098700531, 0.33\n"
    "-0.92966968, 0.35\n"
    "-0.760638829, 0.28\n"
    "-0.591607978, 0.61\n"
    "-0.422577127, 0.38\n"
    "-0.253546276, 0.38\n"
    "-0.084515425, 0.42\n"
    "0.084515425, 0.51\n"
    "0.253546276, 0.6\n"
    "0.422577127, 0.55\n"
    "0.591607978, 0.56\n"
    "0.760638829, 0.53\n"
    "0.92966968, 0.61\n"
    "1.098700531, 0.65\n"
    "1.267731382, 0.68\n"
    "1.436762233, 0.74\n"
    "1.605793084}, 0.87";

static void
init_loader_conf(DLConf *conf) {
  conf->options = DL_INSERT_ONE | DL_MEM_BASED;
  conf->x_dim = 2;
  conf->x_cols[1] = 0;
  conf->y_col = 1;
  conf->mem = data;
  conf->mem_size = sizeof(data);
}
