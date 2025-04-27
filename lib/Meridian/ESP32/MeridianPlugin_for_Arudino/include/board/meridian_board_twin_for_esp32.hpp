/**
 * @file meridian_board_twin_for_esp32.hpp
 * @brief "Meridian Board Twin for ESP32"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__

#include "board/pins/meridian_board_twin_for_esp32_pins.hpp"
#include "board/setting/meridian_board_twin_for_esp32_setting.hpp"

namespace meridian {
namespace board {

class MeridianBoardTwinForEsp32 {
public:
  MeridianBoardTwinForEsp32() = default;
  ~MeridianBoardTwinForEsp32() = default;

  bool Setup() {
    return true; // Setup successful
  }
  bool Loop() {
    return true; // Loop successful
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__
