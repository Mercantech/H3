/*
 * test_temp_color.cpp – Simple unit tests for tempToRgb (demo af pio test)
 *
 * Kør på PC (native):  pio test -e native
 * Tester kun den rene logik i temp_color – ingen Arduino/hardware.
 */

#include <unity.h>
#include "temp_color.h"

void setUp(void) {}
void tearDown(void) {}

void test_kold_temperatur_giver_blaa(void) {
  int r, g, b;
  tempToRgb(10.0f, r, g, b);
  TEST_ASSERT_EQUAL_INT(0, r);
  TEST_ASSERT_EQUAL_INT(50, g);
  TEST_ASSERT_EQUAL_INT(255, b);
}

void test_varm_temperatur_giver_roed(void) {
  int r, g, b;
  tempToRgb(25.0f, r, g, b);
  TEST_ASSERT_EQUAL_INT(255, r);
  TEST_ASSERT_EQUAL_INT(195, g);  // 255 - (25-22)*20
  TEST_ASSERT_EQUAL_INT(0, b);
}

void test_mellem_temperatur_giver_overgang(void) {
  int r, g, b;
  tempToRgb(18.5f, r, g, b);  // mellem 15 og 22
  TEST_ASSERT_TRUE(r >= 0 && r <= 255);
  TEST_ASSERT_EQUAL_INT(100, g);
  TEST_ASSERT_TRUE(b >= 0 && b <= 255);
  TEST_ASSERT_TRUE(r + b <= 255 + 50);  // sanity
}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_kold_temperatur_giver_blaa);
  RUN_TEST(test_varm_temperatur_giver_roed);
  RUN_TEST(test_mellem_temperatur_giver_overgang);
  return UNITY_END();
}
