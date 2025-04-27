#if defined(Meridian_LITE_M5StackAtom)

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
  M5.begin(true, false, true);
  M5.dis.begin();
  // M5.dis.fillpix(CRGB::White);
  // delay(1000);
  M5.dis.fillpix(CRGB::Green);

  Serial.println("==========================================");
  Serial.println("Hello M5Stack Atom!");
  Serial.println("Version: " MERIDIAN_VERSION);
  // Serial.println("Build Time: " BUILD_TIME);

  Serial.printf("Formatted Build Time: %s", GetTimeString(BUILD_TIME).c_str());
}

//==================================================================================================
// MAIN LOOP
//==================================================================================================
void loop() {
  exit(0);
}

#endif // Meridian_LITE_M5StackAtom
