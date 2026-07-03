/*
 * test_temperature_color.cpp – Unit tests for temperatureToRgb
 *
 * Run on PC: pio test -e native
 */

#include <unity.h>

#include "temperature_color.h"

void setUp(void) {}
void tearDown(void) {}

void test_cold_temperature_returns_blue(void) {
  int red = 0;
  int green = 0;
  int blue = 0;
  temperatureToRgb(10.0f, red, green, blue);
  TEST_ASSERT_EQUAL_INT(0, red);
  TEST_ASSERT_EQUAL_INT(50, green);
  TEST_ASSERT_EQUAL_INT(255, blue);
}

void test_warm_temperature_returns_red(void) {
  int red = 0;
  int green = 0;
  int blue = 0;
  temperatureToRgb(25.0f, red, green, blue);
  TEST_ASSERT_EQUAL_INT(255, red);
  TEST_ASSERT_EQUAL_INT(195, green);
  TEST_ASSERT_EQUAL_INT(0, blue);
}

void test_mid_temperature_returns_blend(void) {
  int red = 0;
  int green = 0;
  int blue = 0;
  temperatureToRgb(18.5f, red, green, blue);
  TEST_ASSERT_TRUE(red >= 0 && red <= 255);
  TEST_ASSERT_EQUAL_INT(100, green);
  TEST_ASSERT_TRUE(blue >= 0 && blue <= 255);
  TEST_ASSERT_TRUE(red + blue <= 255 + 50);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_cold_temperature_returns_blue);
  RUN_TEST(test_warm_temperature_returns_red);
  RUN_TEST(test_mid_temperature_returns_blend);
  return UNITY_END();
}
