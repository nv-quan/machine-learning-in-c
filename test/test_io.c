#include "test_io.h"

#include <string.h>

#include "config.h"
#include "data.h"
#include "io.h"
#include "test_config.h"
#include "utils.h"

void init_dlconf(DLConf* conf);

int
test_point_io(char* name) {
  DLConf conf;
  Point point;
  int n;
  double y = 37.9f;
  double x[] = {32.0f, 84.87882f, 10.0f};
  int i;

  strcpy(name, "Test IO");
  init_dlconf(&conf);
  DatLoader* loader = make_data_loader(&conf);
  if ((n = load_data(loader, 1, &point)) != 1) {
    fprintf(stderr, "test_point_io: expect load_data to return 1, actual: %d\n",
            n);
    return 0;
  }
  if (ld_err(loader)) {
    fprintf(stderr,
            "test_point_io: expect load_data to not have error, actual: %d\n",
            loader->err);
    return 0;
  }
  if (point.x_length != TCF_FEAT_DIM) {
    fprintf(stderr,
            "test_point_io: expect point's dimension to be %d, actual: %lu\n",
            TCF_FEAT_DIM, point.x_length);
    return 0;
  }
  if (!double_eq(point.y, y)) {
    fprintf(stderr, "test_point_io: expect point's y to be %lf, actual: %lf\n",
            y, point.y);
    return 0;
  }
  for (i = 0; i < 3; ++i) {
    if (!double_eq(point.x[i], x[i])) {
      fprintf(stderr,
              "test_point_io: expect point's x[%d] to be %lf, actual: %lf\n", i,
              x[i], point.x[i]);
    }
  }
  destroy_dat_loader(loader);
  return 1;
}

void
init_dlconf(DLConf* conf) {
  int i;
  int feature_columns[] = {2, 3, 4};

  snprintf(conf->file_path, 1000, "%s", TCF_CSV_PATH);
  conf->x_dim = TCF_FEAT_DIM;
  for (i = 0; i < TCF_FEAT_DIM; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = TCF_OUTPUT_COL;
  conf->has_header = 1;
}
