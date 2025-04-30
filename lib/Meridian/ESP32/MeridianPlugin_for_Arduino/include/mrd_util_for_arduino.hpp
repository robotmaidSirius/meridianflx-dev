/**
 * @file mrd_util_for_arduino.hpp
 * @brief
 * @version 1.0.0
 * @date 2025-04-29
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MRD_UTIL_FOR_ARDUINO_HPP__
#define __MRD_UTIL_FOR_ARDUINO_HPP__

#include <Arduino.h>

String GetTimeString(int unixTime) {
  // UnixTimeが指定している`BUILD_TIME`をyyyy/mm/dd hh:mm:ss形式に変換する
  time_t buildTime = BUILD_TIME; // BUILD_TIMEはUnixTime形式で定義されていると仮定
  struct tm *timeInfo = gmtime(&buildTime);
  char formattedTime[25];
  snprintf(formattedTime, sizeof(formattedTime), "%04d/%02d/%02d %02d:%02d:%02d",
           (timeInfo->tm_year + 1900), // yy
           timeInfo->tm_mon + 1,       // mm
           timeInfo->tm_mday,          // dd
           timeInfo->tm_hour,          // hh
           timeInfo->tm_min,           // mm
           timeInfo->tm_sec);          // ss
  return String(formattedTime);
}

#endif // __MRD_UTIL_FOR_ARDUINO_HPP__
