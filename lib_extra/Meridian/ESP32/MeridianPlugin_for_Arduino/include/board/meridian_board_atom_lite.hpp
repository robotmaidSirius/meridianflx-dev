/**
 * @file meridian_board_atom_lite.hpp
 * @brief "Meridian Board on M5Stack - AtomLite"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_ATOM_LITE_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_ATOM_LITE_HPP__
// ヘッダファイルの読み込み
#include "meridian_plugin_settings.hpp"
// ライブラリ導入
#include <Arduino.h>
#include <interface/i_mrd_board.hpp>

namespace meridian {
namespace board {

class MeridianBoardOnAtomLite : public IMrdBoard {
public:
  class Parameter {
  public:
  };
  Parameter parameter;

protected:
  /// @brief ループ時の待機処理
  int waiting() override {
    int wait_time_ms = SETTING_DEFAULT_DELAY_TIME_MS;
    delayMicroseconds(wait_time_ms * 1000); // 追加の待機（マイクロ秒単位）
    return wait_time_ms * 1000;
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_ATOM_LITE_HPP__
