/**
 * @file main_lite_m5stack_atomlite.cpp
 * @brief Meridian LITE [Board: M5Stack-AtomLite]のメインルーチン
 * @version 0.1.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_LITE_M5StackAtomLite)
#include "board_lite_m5stack_atomlite.hpp"
#include <M5Atom.h>

meridian::BoardSetting a_conductor;

//==================================================================================================
//  SETUP
//==================================================================================================
void setup() {
  //////////////////////////////////////////////////////////
  // M5Stack Atomの初期化
  //////////////////////////////////////////////////////////
  M5.begin(true, true, true);
  M5.dis.begin();
  M5.dis.fillpix(CRGB::Yellow);
  delay(2000);
  M5.dis.fillpix(CRGB::Green);

  //////////////////////////////////////////////////////////
  // setupの完了したら、LEDを青色に変更
  //////////////////////////////////////////////////////////
  if (false == a_conductor.begin()) {
    while (true) {
      Serial.println("Error: Board Setup failed.");
      sleep(3000);
    }
  }
  M5.dis.fillpix(CRGB::Blue);
}

//==================================================================================================
// MAIN LOOP
//==================================================================================================
void loop() {
  M5.update();
  a_conductor.update();
}

#endif
