/**
 * @file main_twin_esp32.cpp
 * @brief Meridian TWIN [Board: ESP32]のメインルーチン
 * @version 0.1.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_TWIN_ESP32)
#include "board_twin_esp32.hpp"
meridian::BoardSetting a_conductor;

//==================================================================================================
//  SETUP
//==================================================================================================
void setup() {
  //////////////////////////////////////////////////////////
  // setupの完了したら、LEDを青色に変更
  //////////////////////////////////////////////////////////
  if (false == a_conductor.begin()) {
    while (true) {
      Serial.println("Error: Board Setup failed.");
      sleep(3000);
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
