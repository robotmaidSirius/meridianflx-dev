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

meridian::board::MeridianBoardAtom board;

//==================================================================================================

/// @brief バージョン情報の定義
#define MERIDIAN_VERSION BUILD_BOARD_NAME " ver." BUILD_VERSION

#include <Arduino.h>
#include <M5Atom.h>

String GetTimeString(int unixTime) {
  // UnixTimeが指定している`BUILD_TIME`をyyyy/mm/dd hh:mm:ss形式に変換する
  time_t buildTime = BUILD_TIME; // BUILD_TIMEはUnixTime形式で定義されていると仮定
  struct tm *timeInfo = gmtime(&buildTime);
  char formattedTime[16];
  snprintf(formattedTime, sizeof(formattedTime), "%04d/%02d/%02d/ %02d:%02d:%02d",
           (timeInfo->tm_year + 1900), // yy
           timeInfo->tm_mon + 1,       // mm
           timeInfo->tm_mday,          // dd
           timeInfo->tm_hour,          // hh
           timeInfo->tm_min,           // mm
           timeInfo->tm_sec);          // ss
  return String(formattedTime);
}

//==================================================================================================
//  SETUP
//==================================================================================================
void setup() {
  M5.begin(true, true, true);
  M5.dis.begin();
  M5.dis.fillpix(CRGB::Green);

  Serial.println("==========================================");
  Serial.println("Hello M5Stack Atom!");
  Serial.println("Version: " MERIDIAN_VERSION);
  // Serial.println("Build Time: " BUILD_TIME);

  Serial.printf("Formatted Build Time: %s", GetTimeString(BUILD_TIME).c_str());

  board.Setup();
}

//==================================================================================================
// MAIN LOOP
//==================================================================================================
void loop() {
  Meridim90 a_meridim;

  if (true == board.Input(a_meridim)) {
    // アプリ処理を記載する

    board.Output(a_meridim);
  }
  exit(0);
}

#endif // Meridian_LITE_M5StackAtom
