/**
 * @file meridian_board_twin_for_teensy40.hpp
 * @brief "Meridian Board Twin for teensy40"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_TEENSY40_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_TEENSY40_HPP__

#include "board/pins/meridian_board_twin_for_teensy40_pins.hpp"
#include "board/setting/meridian_board_twin_for_teensy40_setting.hpp"

namespace meridian {
namespace board {

class MeridianBoardTwinForTeensy40 {
public:
  MeridianBoardTwinForTeensy40() = default;
  ~MeridianBoardTwinForTeensy40() = default;

  bool Setup() {
    return true; // Setup successful
  }
  bool Loop() {
    return true; // Loop successful
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_TEENSY40_HPP__
