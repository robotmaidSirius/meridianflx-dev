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
#include <interface/i_board.hpp>

namespace meridian {
namespace board {

class MeridianBoardTwinForTeensy40 : public IBoard {
public:
  MeridianBoardTwinForTeensy40() {}
  ~MeridianBoardTwinForTeensy40() {}

  bool begin() override { return true; }
  bool input(Meridim90 &a_meridim) override { return true; }
  // bool Process(Meridim90 &a_meridim) { return true; }
  bool output(Meridim90 &a_meridim) override { return true; }
  void waiting() override {}
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_TEENSY40_HPP__
