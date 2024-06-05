#include "test_io.h"

#include <string.h>

#include "config.h"
#include "data.h"
#include "io.h"
#include "test_config.h"
#include "utils.h"

void init_dlconf_file(DLConf *conf);
void init_dlconf_mem(DLConf *conf);

int
test_point_io_file(char *name) {
  DLConf conf;
  Point point;
  int n, i, retval;
  double y = 37.9f, x[] = {32.0f, 84.87882f, 10.0f};

  strcpy(name, "Test point io file");
  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  if ((n = load_data(loader, 1, &point)) != 1) {
    fprintf(stderr,
            "test_point_io_file: expect load_data to return 1, actual: %d\n",
            n);
    retval = 0;
    goto cleanup;
  }
  if (ld_err(loader)) {
    fprintf(
        stderr,
        "test_point_io_file: expect load_data to not have error, actual: %d\n",
        loader->err);
    retval = 0;
    goto cleanup;
  }
  if (point.x_length != TCF_FEAT_DIM) {
    fprintf(
        stderr,
        "test_point_io_file: expect point's dimension to be %d, actual: %lu\n",
        TCF_FEAT_DIM, point.x_length);
    retval = 0;
    goto cleanup;
  }
  if (!double_eq(point.y, y)) {
    fprintf(stderr,
            "test_point_io_file: expect point's y to be %lf, actual: %lf\n", y,
            point.y);
    retval = 0;
    goto cleanup;
  }
  for (i = 0; i < 3; ++i) {
    if (!double_eq(point.x[i], x[i])) {
      fprintf(
          stderr,
          "test_point_io_file: expect point's x[%d] to be %lf, actual: %lf\n",
          i, x[i], point.x[i]);
      retval = 0;
      goto cleanup;
    }
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

int
test_point_io_mem(char *name) {
  DLConf conf;
  Point point;
  int n, i, retval;
  double y = 37.9f, x[] = {32.0f, 84.87882f, 10.0f};

  strcpy(name, "Test point io mem");
  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  if ((n = load_data(loader, 1, &point)) != 1) {
    fprintf(stderr,
            "test_point_io_mem: expect load_data to return 1, actual: %d\n", n);
    retval = 0;
    goto cleanup;
  }
  if (ld_err(loader)) {
    fprintf(
        stderr,
        "test_point_io_mem: expect load_data to not have error, actual: %d\n",
        loader->err);
    retval = 0;
    goto cleanup;
  }
  if (point.x_length != TCF_FEAT_DIM) {
    fprintf(
        stderr,
        "test_point_io_mem: expect point's dimension to be %d, actual: %lu\n",
        TCF_FEAT_DIM, point.x_length);
    retval = 0;
    goto cleanup;
  }
  if (!double_eq(point.y, y)) {
    fprintf(stderr,
            "test_point_io_mem: expect point's y to be %lf, actual: %lf\n", y,
            point.y);
    retval = 0;
    goto cleanup;
  }
  for (i = 0; i < 3; ++i) {
    if (!double_eq(point.x[i], x[i])) {
      fprintf(
          stderr,
          "test_point_io_mem: expect point's x[%d] to be %lf, actual: %lf\n", i,
          x[i], point.x[i]);
      retval = 0;
      goto cleanup;
    }
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

int
test_continuous_loader_file1(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;

  strcpy(name, "Test continous loader file 1");
  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 1, &point1)) > 0; i++) {
  }
  if (i != 414) {
    fprintf(stderr,
            "test_continuous_loader_file1: wrong number of points, expect %d, "
            "actual: %d\n",
            414, i);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

int
test_continuous_loader_file2(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;

  strcpy(name, "Test continous loader file 2");
  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0; i++) {
  }
  if (i != 207) {
    fprintf(stderr,
            "test_continuous_loader_file2: wrong number of points, expect %d, "
            "actual: %d\n",
            207, i);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

/* Example no.20: 20, 2012.667, 1.5, 23.38284,7,24.96772,121.54102,47.7 */
int
test_continuous_loader_file3(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;
  double x[] = {1.5, 23.38284, 7}, y = 47.7;

  strcpy(name, "Test continous loader file 3");
  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0 && i < 20; i++) {
  }

  if (x[0] != point1.x[0] || x[1] != point1.x[1] || x[2] != point1.x[2]) {
    fprintf(stderr,
            "test_continuous_loader_file3: wrong point 20, expect x1 = %lf, x2 "
            "= %lf, x3 = %lf, y = %lf, actual x1 = %lf, x2 = %lf, x3 = %lf, y "
            "= %lf\n",
            x[0], x[1], x[2], y, point1.x[0], point1.x[1], point1.x[2],
            point1.y);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

int
test_continuous_loader_mem1(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;

  strcpy(name, "Test continous loader mem 1");
  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 1, &point1)) > 0; i++) {
  }
  if (i != 6) {
    fprintf(stderr,
            "test_continuous_loader_mem1: wrong number of points, expect %d, "
            "actual: %d\n",
            6, i);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

int
test_continuous_loader_mem2(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;

  strcpy(name, "Test continous loader mem 2");
  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0; i++) {
  }
  if (i != 3) {
    fprintf(stderr,
            "test_continuous_loader_mem2: wrong number of points, expect %d, "
            "actual: %d\n",
            3, i);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

/* Example no. 4: 4,2013.500,13.3,561.9845,5,24.98746,121.54391,54.8 */
int
test_continuous_loader_mem3(char *name) {
  DLConf conf;
  Point point1;
  int n, i, retval;
  double x[] = {13.3, 561.9845, 5}, y = 54.8;

  strcpy(name, "Test continous loader mem 3");
  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0 && i < 4; i++) {
  }

  if (x[0] != point1.x[0] || x[1] != point1.x[1] || x[2] != point1.x[2]) {
    fprintf(stderr,
            "test_continuous_loader_mem3: wrong point 4, expect x1 = %lf, x2 = "
            "%lf, x3 = %lf, y = %lf, actual x1 = %lf, x2 = %lf, x3 = %lf, y = "
            "%lf\n",
            x[0], x[1], x[2], y, point1.x[0], point1.x[1], point1.x[2],
            point1.y);
    retval = 0;
    goto cleanup;
  }
  retval = 1;
cleanup:
  destroy_dat_loader(loader);
  return retval;
}

void
init_dlconf_file(DLConf *conf) {
  int i;
  int feature_columns[] = {2, 3, 4};

  snprintf(conf->file_path, 1000, "%s", TCF_CSV_PATH);
  conf->x_dim = 3;
  for (i = 0; i < 3; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = 7;
  conf->has_header = 1;
  conf->is_mem = 0;
}

void
init_dlconf_mem(DLConf *conf) {
  int i;
  int feature_columns[] = {2, 3, 4};
  char mem[] =
      "1,2012.917,32,84.87882,10,24.98298,121.54024,37.9\n2,2012.917,19.5,306."
      "5947,9,24.98034,121.53951,42.2\n3,2013.583,13.3,561.9845,5,24.98746,121."
      "54391,47.3\n4,2013.500,13.3,561.9845,5,24.98746,121.54391,54.8\n5,2012."
      "833,5,390.5684,5,24.97937,121.54245,43.1\n6,2012.667,7.1,2175.03,3,24."
      "96305,121.51254,32.1";

  conf->x_dim = 3;
  for (i = 0; i < 3; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = 7;
  conf->has_header = 0;
  conf->is_mem = 1;
  conf->mem = (void *)mem;
  conf->mem_size = sizeof(mem);
}
