/**
 * @file communication_diagnostic.cpp
 * @brief テストコード / ログ出力
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#include <unity.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void test_setup(void) {
  bool result = true;

  TEST_ASSERT_TRUE(result);
}

void test_loop(void) {
  int result = 1;

  TEST_ASSERT_EQUAL(1, result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RUN_UNITY_TESTS() {
  UNITY_BEGIN();
  //////////////////////////////////
  RUN_TEST(test_setup);
  RUN_TEST(test_loop);
  //////////////////////////////////
  UNITY_END();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ARDUINO
#include <Arduino.h>
void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  RUN_UNITY_TESTS();
  exit(0);
}
void loop() {
}
#else
int main(int argc, char **argv) {
  RUN_UNITY_TESTS();
  return 0;
}
#endif
