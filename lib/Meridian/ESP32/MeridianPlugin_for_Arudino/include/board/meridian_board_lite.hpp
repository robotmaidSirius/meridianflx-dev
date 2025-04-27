/**
 * @file meridian_board_lite.hpp
 * @brief "Meridian Board -Lite-"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_HPP__

#include "board/pins/meridian_board_lite_pins.hpp"
#include "board/setting/meridian_board_lite_setting.hpp"

namespace meridian {
namespace board {

class MeridianBoardLite {
public:
  MeridianBoardLite() = default;
  ~MeridianBoardLite() = default;

  bool Setup() {
    return true; // Setup successful
  }
  bool Loop() {
    return true; // Loop successful
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_HPP__
