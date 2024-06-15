#include "test_io.h"

#include <check.h>
#include <string.h>

#include "config.h"
#include "data.h"
#include "io.h"
#include "test_config.h"
#include "utils.h"

void init_dlconf_file(DLConf *conf);
void init_dlconf_mem(DLConf *conf);

static const char example_mem[] =
    "1,2012.917,32,84.87882,10,24.98298,121.54024,37.9\n2,2012.917,19.5,306."
    "5947,9,24.98034,121.53951,42.2\n3,2013.583,13.3,561.9845,5,24.98746,121."
    "54391,47.3\n4,2013.500,13.3,561.9845,5,24.98746,121.54391,54.8\n5,2012."
    "833,5,390.5684,5,24.97937,121.54245,43.1\n6,2012.667,7.1,2175.03,3,24."
    "96305,121.51254,32.1";

START_TEST(test_point_io_file) {
  DLConf conf;
  Point point;
  int n, i;
  double y = 37.9f, x[] = {32.0f, 84.87882f, 10.0f};

  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  ck_assert_msg((n = load_data(loader, 1, &point)) == 1,
                "Expect load_data to return 1, actual: %d", n);
  ck_assert_msg(!ld_err(loader),
                "Expect load_data to not have error, actual: %d", loader->err);
  ck_assert_msg(point.x_length == TCF_FEAT_DIM,
                "Expect point's dimension to be %d, actual: %lu", TCF_FEAT_DIM,
                point.x_length);
  ck_assert_msg(double_eq(point.y, y),
                "Expect point's y to be %lf, actual: %lf", y, point.y);
  for (i = 0; i < 3; ++i) {
    ck_assert_msg(double_eq(point.x[i], x[i]),
                  "Expect point's x[%d] to be %lf, actual: %lf", i, x[i],
                  point.x[i]);
  }
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_point_io_file_insert_one) {
  DLConf conf;
  Point point;
  int n, i, feature_columns[] = {2, 3, 4};
  double y = 37.9f, x[] = {1.0f, 32.0f, 84.87882f, 10.0f};

  strcpy(conf.file_path, TCF_CSV_PATH);
  conf.x_dim = 4;
  for (i = 0; i < 3; ++i) {
    conf.x_cols[i + 1] = feature_columns[i];
  }
  conf.y_col = 7;
  conf.options = DL_HAS_HEADER | DL_INSERT_ONE;
  DatLoader *loader = make_data_loader(&conf);
  ck_assert_msg((n = load_data(loader, 1, &point)) == 1,
                "Expect load_data to return 1, actual: %d", n);
  ck_assert_msg(!ld_err(loader),
                "Expect load_data to not have error, actual: %d", loader->err);
  ck_assert_msg(point.x_length == 4,
                "Expect point's dimension to be %d, actual: %lu", 4,
                point.x_length);
  ck_assert_msg(double_eq(point.y, y),
                "Expect point's y to be %lf, actual: %lf", y, point.y);
  for (i = 0; i < 4; ++i) {
    ck_assert_msg(double_eq(point.x[i], x[i]),
                  "Expect point's x[%d] to be %lf, actual: %lf", i, x[i],
                  point.x[i]);
  }
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_point_io_mem_insert_one) {
  DLConf conf;
  Point point;
  int n, i, feature_columns[] = {2, 3, 4};
  double y = 37.9f, x[] = {1.0f, 32.0f, 84.87882f, 10.0f};

  conf.x_dim = 4;
  for (i = 0; i < 3; ++i) {
    conf.x_cols[i + 1] = feature_columns[i];
  }
  conf.y_col = 7;
  conf.options = DL_MEM_BASED | DL_INSERT_ONE;
  conf.mem = (void *)example_mem;
  conf.mem_size = sizeof(example_mem);
  DatLoader *loader = make_data_loader(&conf);
  ck_assert_msg((n = load_data(loader, 1, &point)) == 1,
                "Expect load_data to return 1, actual: %d", n);
  ck_assert_msg(!ld_err(loader),
                "Expect load_data to not have error, actual: %d", loader->err);
  ck_assert_msg(point.x_length == 4,
                "Expect point's dimension to be %d, actual: %lu", 4,
                point.x_length);
  ck_assert_msg(double_eq(point.y, y),
                "Expect point's y to be %lf, actual: %lf", y, point.y);
  for (i = 0; i < 4; ++i) {
    ck_assert_msg(double_eq(point.x[i], x[i]),
                  "Expect point's x[%d] to be %lf, actual: %lf", i, x[i],
                  point.x[i]);
  }
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_point_io_mem) {
  DLConf conf;
  Point point;
  int n, i;
  double y = 37.9f, x[] = {32.0f, 84.87882f, 10.0f};

  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  ck_assert_msg((n = load_data(loader, 1, &point)) == 1,
                "Expect load_data to return 1, actual: %d", n);
  ck_assert_msg(!ld_err(loader),
                "Expect load_data to not have error, actual: %d", loader->err);
  ck_assert_msg(point.x_length == TCF_FEAT_DIM,
                "Expect point's dimension to be %d, actual: %lu", TCF_FEAT_DIM,
                point.x_length);
  ck_assert_msg(double_eq(point.y, y),
                "Expect point's y to be %lf, actual: %lf", y, point.y);
  for (i = 0; i < 3; ++i) {
    ck_assert_msg(double_eq(point.x[i], x[i]),
                  "Expect point's x[%d] to be %lf, actual: %lf", i, x[i],
                  point.x[i]);
  }
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_file1) {
  DLConf conf;
  Point point1;
  int n, i;

  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 1, &point1)) > 0; i++) {
  }
  ck_assert_msg(i == 414, "Wrong number of points, expect %d, actual: %d", 414,
                i);
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_file2) {
  DLConf conf;
  Point point1;
  int n, i;

  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0; i++) {
  }
  ck_assert_msg(i == 207, "Wrong number of points, expect %d, actual: %d", 207,
                i);
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_file3) {
  DLConf conf;
  Point point1;
  int n, i;
  double x[] = {1.5, 23.38284, 7}, y = 47.7;

  init_dlconf_file(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; i < 20 && (n = load_data(loader, 1, &point1)) > 0; i++) {
  }

  ck_assert_msg(
      x[0] == point1.x[0] && x[1] == point1.x[1] && x[2] == point1.x[2],
      "Wrong point 20, expect x1 = %lf, x2 = %lf, x3 = %lf, y = %lf, actual x1 "
      "= %lf, x2 = %lf, x3 = %lf, y = %lf",
      x[0], x[1], x[2], y, point1.x[0], point1.x[1], point1.x[2], point1.y);
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_mem1) {
  DLConf conf;
  Point point1;
  int n, i;

  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 1, &point1)) > 0; i++) {
  }
  ck_assert_msg(i == 6, "Wrong number of points, expect %d, actual: %d", 6, i);
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_mem2) {
  DLConf conf;
  Point point1;
  int n, i;

  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; (n = load_data(loader, 2, &point1)) > 0; i++) {
  }
  ck_assert_msg(i == 3, "Wrong number of points, expect %d, actual: %d", 3, i);
  destroy_dat_loader(loader);
}
END_TEST

START_TEST(test_continuous_loader_mem3) {
  DLConf conf;
  Point point1;
  int n, i;
  double x[] = {13.3, 561.9845, 5}, y = 54.8;

  init_dlconf_mem(&conf);
  DatLoader *loader = make_data_loader(&conf);
  for (i = 0; i < 4 && (n = load_data(loader, 1, &point1)) > 0; i++) {
  }

  ck_assert_msg(
      x[0] == point1.x[0] && x[1] == point1.x[1] && x[2] == point1.x[2],
      "Wrong point 4, expect x1 = %lf, x2 = %lf, x3 = %lf, y = %lf, actual x1 "
      "= %lf, x2 = %lf, x3 = %lf, y = %lf",
      x[0], x[1], x[2], y, point1.x[0], point1.x[1], point1.x[2], point1.y);
  destroy_dat_loader(loader);
}
END_TEST

Suite *
io_suite(void) {
  Suite *s = suite_create("Data");

  TCase *tc_point_io = tcase_create("Point IO");
  tcase_add_test(tc_point_io, test_point_io_file);
  tcase_add_test(tc_point_io, test_point_io_file_insert_one);
  tcase_add_test(tc_point_io, test_point_io_mem_insert_one);
  tcase_add_test(tc_point_io, test_point_io_mem);
  suite_add_tcase(s, tc_point_io);

  TCase *tc_continuous_loader = tcase_create("Continuous Loader");
  tcase_add_test(tc_continuous_loader, test_continuous_loader_file1);
  tcase_add_test(tc_continuous_loader, test_continuous_loader_file2);
  tcase_add_test(tc_continuous_loader, test_continuous_loader_file3);
  tcase_add_test(tc_continuous_loader, test_continuous_loader_mem1);
  tcase_add_test(tc_continuous_loader, test_continuous_loader_mem2);
  tcase_add_test(tc_continuous_loader, test_continuous_loader_mem3);
  suite_add_tcase(s, tc_continuous_loader);

  return s;
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
  conf->options = DL_HAS_HEADER;
}

void
init_dlconf_mem(DLConf *conf) {
  int i;
  int feature_columns[] = {2, 3, 4};
  conf->x_dim = 3;

  for (i = 0; i < 3; ++i) {
    conf->x_cols[i] = feature_columns[i];
  }
  conf->y_col = 7;
  conf->options = DL_MEM_BASED;
  conf->mem = (void *)example_mem;
  conf->mem_size = sizeof(example_mem);
}
