/**
 * @file main_twin_teensy40.cpp
 * @brief Meridian TWIN [Board: Teensy 4.0]のメインルーチン
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_TWIN_Teensy40)
#include "board_twin_teensy40.hpp"
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
