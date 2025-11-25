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

namespace meridian {
namespace board {

class MeridianBoardTwinForEsp32 {
public:
  class Parameter {
  public:
  };
  Parameter parameter;

public:
  bool begin() {
    // 初期化処理をここに記述
    return this->setup();
  }
  bool update() {
    // 定期的な更新処理をここに記述
    return this->loop();
  }

protected:
  /// @brief 初期化を実行する
  virtual bool setup() {
    return true;
  }
  /// @brief 初期化を実行する
  virtual bool loop() {
    return true;
  }

  /// @brief ループ時の待機処理
  void waiting() {
    // do nothing
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_HPP__
