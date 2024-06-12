#include <check.h>
#include <custom_math.h>
#include <stdlib.h>
#include "utils.h"

START_TEST(test_double_eq)
{
  double a = 1.23456;
  double b = 1.23457;
  double c = 1.234561;
  double d = 1.234559;
  ck_assert(double_eq(a, b));
}
END_TEST

Suite * math_suite(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("Math test");
  tc_core = tcase_create("Basic");
  tcase_add_test(tc_core, test_double_eq);
  suite_add_tcase(s, tc_core);
  return s;
}
int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = math_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
