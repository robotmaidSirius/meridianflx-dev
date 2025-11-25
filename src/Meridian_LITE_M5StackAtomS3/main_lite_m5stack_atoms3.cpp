/**
 * @file main_lite_m5stack_atoms3.cpp
 * @brief Meridian LITE [Board: M5Stack-AtomS3]のメインルーチン
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_LITE_M5StackAtomS3)
#include "board_lite_m5stack_atoms3.hpp"
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
