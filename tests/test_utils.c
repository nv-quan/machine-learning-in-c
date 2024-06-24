#include "test_utils.h"

#include <check.h>

START_TEST(test_conf_tree_init) {
  ConfTree new_tree;
  conf_tree_init(&new_tree, "Name", "Value");
  ck_assert_str_eq("Name", new_tree.name);
  ck_assert_str_eq("Value", new_tree.value);
  ck_assert_int_eq(0, new_tree.child_count);
  conf_tree_free(&new_tree);
}
END_TEST

Suite*
utils_suite(void) {
  Suite* s;
  TCase* tc_conf_tree;

  s = suite_create("Utility test");
  tc_conf_tree = tcase_create("Config Tree");
  tcase_add_test(tc_conf_tree, test_conf_tree_init);
  suite_add_tcase(s, tc_conf_tree);
}
