/**
 * @file mrd_string.hpp
 * @brief
 * @version 1.0.0
 * @date 2025-04-29
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_UTILS_MRD_STRING_HPP__
#define __MERIDIAN_UTILS_MRD_STRING_HPP__
// ライブラリ導入
#include <Arduino.h>

namespace meridian {

/// @brief UnixTimeをyyyy/mm/dd hh:mm:ss形式に変換する
String mrd_time_to_string(int unixTime);
/// @brief フォーマットされた文字列を返す
String mrd_format(const char *format, ...);

} // namespace meridian

#endif // __MERIDIAN_UTILS_MRD_STRING_HPP__
