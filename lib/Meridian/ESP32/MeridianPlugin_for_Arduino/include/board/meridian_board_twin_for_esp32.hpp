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
#include <Arduino.h>
#include <interface/i_mrd_board.hpp>

namespace meridian {
namespace board {

class MeridianBoardTwinForEsp32 : public IMrdBoard {
public:
  class Parameter {
  public:
  };
  Parameter parameter;

protected:
  /// @brief ループ時の待機処理
  int waiting() override {
    int wait_time_ms = 10;
    sleep(wait_time_ms);
    return wait_time_ms * 1000;
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__
