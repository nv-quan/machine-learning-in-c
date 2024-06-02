#include "test_io.h"

#include <string.h>

#include "config.h"
#include "io.h"

void init_dlconf(DLConf* conf);

int
test_io(char* name) {
  strcpy(name, "Test IO");
  DLConf conf;
  init_dlconf(&conf);
  DatLoader* loader = make_data_loader(&conf);

  destroy_dat_loader(loader);
  return 1;
}

void
init_dlconf(DLConf* conf) {
  int i;
  int feature_columns[] = CF_FEAT_COLS;

  snprintf(conf->file_path, CF_PATH_LEN, "%s", CF_CSV_PATH);
  conf->x_dim = CF_FEAT_DIM;
  for (i = 0; i < CF_FEAT_DIM; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = CF_OUTPUT_COL;
  conf->has_header = 1;
}
