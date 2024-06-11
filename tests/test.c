#include <check.h>
#include <custom_math.h>

START_TEST(test_double_eq)
{
  double a = 1.23456;
  double b = 1.23457;
  double c = 1.234561;
  double d = 1.234559;
  ck_assert_int_ne(double_eq(a, b), 0);
}
END_TEST
int main()
{
  return 0;
}
