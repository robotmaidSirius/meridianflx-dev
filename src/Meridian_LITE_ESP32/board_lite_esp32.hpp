/**
 * @file board_lite_esp32.hpp
 * @brief Meridian LITE [Board: ESP32]のボード設定ファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __BOARD_LITE_ESP32_HPP__
#define __BOARD_LITE_ESP32_HPP__
#include "app_lite_esp32.hpp"
#include "meridian_network_keys.hpp"
#include "meridian_parameter.hpp"
#include <board/meridian_board_lite_for_esp32.hpp>

namespace meridian {

class BoardSetting : public board::MeridianBoardLiteForEsp32 {
public:
  BoardSetting() {}
  ~BoardSetting() {}

protected:
  /// @brief 初期化を実行する
  bool setup() override {
    // 初期化処理をここに記述
    return true;
  }
  /// @brief 初期化を実行する
  bool loop() override {
    // 定期的な更新処理をここに記述
    return true;
  }
};

} // namespace meridian

#endif // __BOARD_LITE_ESP32_HPP__
