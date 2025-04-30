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
#include <Meridim90.hpp>
#include <interface/i_board.hpp>

namespace meridian {
namespace board {

class MeridianBoardTwinForEsp32 : public IBoard {
public:
  MeridianBoardTwinForEsp32() {}
  ~MeridianBoardTwinForEsp32() {}

  bool setup() override { return true; }
  bool input(Meridim90 &a_meridim) override { return true; }
  bool processing(Meridim90 &a_meridim) override { return true; }
  bool output(Meridim90 a_meridim) override { return true; }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__
