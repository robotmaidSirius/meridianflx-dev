/**
 * @file mrd_string.cpp
 * @brief
 * @version 1.0.0
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "mrd_utils/mrd_string.hpp"

#include <Arduino.h>

#ifndef MRD_UTIL_BUFFER_SIZE
#define MRD_UTIL_BUFFER_SIZE 128
#endif

namespace meridian {

String mrd_time_to_string(int unixTime) {
  // UnixTimeが指定している`MERIDIAN_BUILD_TIME`をyyyy/mm/dd hh:mm:ss形式に変換する
  time_t buildTime = (time_t)unixTime; // MERIDIAN_BUILD_TIMEはUnixTime形式で定義されていると仮定
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

/// @brief フォーマットされた文字列を返す
String mrd_format(const char *format, ...) {
  char loc_buf[MRD_UTIL_BUFFER_SIZE];
  char *message = loc_buf;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  int len = vsnprintf(message, sizeof(loc_buf), format, copy);
  va_end(copy);
  if (len < 0) {
    va_end(arg);
    return "";
  }
  if ((unsigned long long)len >= sizeof(loc_buf)) {
    message = (char *)malloc(len + 1);
    if (message == NULL) {
      va_end(arg);
      return "";
    }
    len = vsnprintf(message, len + 1, format, arg);
  }
  va_end(arg);
  String str(message);
  if (message != loc_buf) {
    free(message);
  }
  return str;
}

} // namespace meridian
