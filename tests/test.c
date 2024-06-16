#include <check.h>
#include <custom_math.h>
#include <stdlib.h>

#include "test_data.h"
#include "test_io.h"
#include "test_math.h"
#include "utils.h"

Suite *
master_suite(void) {
  Suite *s;
  s = suite_create("Master suite");
  return s;
}

int
main(void) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(master_suite());
  srunner_add_suite(sr, io_suite());
  srunner_add_suite(sr, data_suite());
  srunner_add_suite(sr, math_suite());

  srunner_run_all(sr, CK_NORMAL);
  srunner_set_log(sr, "test.log");
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
