/**
 * @file meridian_board_lite_for_pico.hpp
 * @brief "Meridian Board Lite for Raspberry Pi Pico"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_FOR_PICO_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_FOR_PICO_HPP__
// ヘッダファイルの読み込み
#include <interface/i_mrd_board.hpp>
// ライブラリ導入
#include <Arduino.h>

namespace meridian {
namespace board {

class MeridianBoardLiteForPico : public IMrdBoard {
public:
  class Parameter {
  public:
  };
  Parameter parameter;

protected:
  /// @brief ループ時の待機処理
  int waiting() override {
    int wait_time_ms = 10;
    delay(wait_time_ms);
    return wait_time_ms * 1000;
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_FOR_PICO_HPP__
