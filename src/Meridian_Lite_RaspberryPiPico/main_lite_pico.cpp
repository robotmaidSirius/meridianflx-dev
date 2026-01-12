/**
 * @file main_lite_pico.cpp
 * @brief Meridian LITE [Board: Raspberry Pi Pico]のメインルーチン
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_Lite_RaspberryPiPico)
#include "board_lite_pico.hpp"
meridian::BoardSetting a_conductor;

pin_size_t pin_led = LED_BUILTIN;
//==================================================================================================
//  SETUP
//==================================================================================================
void setup() {
  // delay(3000);
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, LOW);
  delay(500);
  digitalWrite(pin_led, HIGH);
  if (false == a_conductor.begin()) {
    while (true) {
      Serial.println("Error: Board Setup failed.");
      delay(3000);
    }
  }
}
//==================================================================================================
// MAIN LOOP
//==================================================================================================

void loop() {
  a_conductor.update();
}

#endif
