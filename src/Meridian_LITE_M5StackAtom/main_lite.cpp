/**
 * @file main_lite.cpp
 * @brief Meridian LITE M5Stack Atomのメインルーチン
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#if defined(Meridian_LITE_M5StackAtom)

#include "app_lite.hpp"
#include <board/meridian_board_atom.hpp>
#include <mrd_communication/mrd_conversation_wifi.hpp>
#include <mrd_communication/mrd_diagnostic_uart.hpp>

using namespace meridian::board;
MeridianBoardAtom board;
MrdDiagnosticUart diag(&Serial);

//==================================================================================================

#include <Arduino.h>
#include <M5Atom.h>

bool process(Meridim90 &a_meridim) {
  return true;
}

//==================================================================================================
//  SETUP
//==================================================================================================
void setup() {
  M5.begin(true, true, true);
  M5.dis.begin();
  M5.dis.fillpix(CRGB::Yellow);
  delay(3000);
  M5.dis.fillpix(CRGB::Green);

  board.event_process = process;
  board.push_communication(new MrdConversationWifi(), &diag);
  // board.parameter.delay_time_ms = 1000;
  IMeridianPlugin a;
  board.push_module(&a);
  board.push_module(new IMeridianPlugin);

  if (false == board.begin()) {
    while (true) {
      Serial.println("Error: Board Setup failed.");
      sleep(3000);
    }
  }
  M5.dis.fillpix(CRGB::Green);
}

//==================================================================================================
// MAIN LOOP
//==================================================================================================
void loop() {
  Meridim90 a_meridim;

  if (true == board.loop(a_meridim)) {
    // 最後の処理を記載します。
  }
}

#endif // Meridian_LITE_M5StackAtom
