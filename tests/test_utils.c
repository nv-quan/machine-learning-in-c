#include "test_utils.h"

#include <check.h>

START_TEST(test_double_eq) {
  double a = 1.23456;
  double b = 1.23457;
  double c = 1.234561;
  double d = 1.234559;
  ck_assert(!double_eq(a, b));
  ck_assert(double_eq(a, c));
  ck_assert(double_eq(c, d));
}
END_TEST

START_TEST(test_conf_tree_init) {
  ConfTree new_tree;
  int res;
  res = conf_tree_init(&new_tree, "Name", "Value");
  ck_assert_int_eq(res, 0);
  ck_assert_str_eq("Name", new_tree.name);
  ck_assert_str_eq("Value", new_tree.value);
  ck_assert_int_eq(0, new_tree.child_count);
  conf_tree_free(&new_tree);
}
END_TEST

START_TEST(test_conf_tree_add) {
  ConfTree new_tree, *current;
  int res;
  res = conf_tree_init(&new_tree, "Name", "Value");
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(0, new_tree.child_count);
  current = conf_tree_add(&new_tree, "Child1", "");
  ck_assert_ptr_nonnull(current);
  current = conf_tree_add(&new_tree, "Child2", "");
  ck_assert_ptr_nonnull(current);
  current = conf_tree_add(current, "SubChild1", "SubChildValue");
  ck_assert_ptr_nonnull(current);
  ck_assert_int_eq(new_tree.child_count, 2);
  ck_assert_ptr_nonnull(new_tree.children);
  ck_assert_str_eq("Child1", new_tree.children[0].name);
  ck_assert_str_eq("Child2", new_tree.children[1].name);
  ck_assert_int_eq(new_tree.children[0].child_count, 0);
  ck_assert_int_eq(new_tree.children[1].child_count, 1);
  ck_assert_ptr_nonnull(new_tree.children[1].children);
  ck_assert_str_eq("SubChild1", new_tree.children[1].children[0].name);
  ck_assert_str_eq("SubChildValue", new_tree.children[1].children[0].value);
}
END_TEST

START_TEST(test_s_strcpy) {
  char a[] = "This is a test string";
  char b[21], c[22];
  int is_trunc = 0;
  s_strcpy(b, a, 21, &is_trunc);
  ck_assert_int_ne(is_trunc, 0);
  ck_assert_str_eq(b, "This is a test strin");
  s_strcpy(c, a, 22, &is_trunc);
  ck_assert_int_eq(is_trunc, 0);
  ck_assert_str_eq(c, a);
}
END_TEST

Suite*
utils_suite(void) {
  Suite* s;
  TCase *tc_conf_tree, *tc_s_strcpy, *tc_double_eq;

  s = suite_create("Utility test");

  tc_double_eq = tcase_create("Double equal");
  tcase_add_test(tc_double_eq, test_double_eq);
  suite_add_tcase(s, tc_double_eq);

  tc_s_strcpy = tcase_create("String copy");
  tcase_add_test(tc_s_strcpy, test_s_strcpy);
  suite_add_tcase(s, tc_s_strcpy);

  tc_conf_tree = tcase_create("Config Tree");
  tcase_add_test(tc_conf_tree, test_conf_tree_init);
  tcase_add_test(tc_conf_tree, test_conf_tree_add);
  suite_add_tcase(s, tc_conf_tree);
  return s;
}
