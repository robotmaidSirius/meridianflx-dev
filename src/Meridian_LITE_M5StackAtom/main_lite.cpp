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

//==================================================================================================

#include <Arduino.h>
#include <M5Atom.h>

void Processing(Meridim90 &a_meridim) {
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

  board.event_process = Processing;
  board.plugin.con = new MrdConversationWifi();
  board.plugin.diag = new MrdDiagnosticUart(&Serial);
  board.plugin.pad = nullptr;

  if (false == board.setup()) {
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

  if (true == board.input(a_meridim)) {
    // アプリ処理を記載する

    board.output(a_meridim);
  }
}

#endif // Meridian_LITE_M5StackAtom
