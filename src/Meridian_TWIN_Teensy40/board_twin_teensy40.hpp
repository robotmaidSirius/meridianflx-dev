/**
 * @file board_twin_teensy40.hpp
 * @brief Meridian TWIN [Board: Teensy 4.0]のボード設定ファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __BOARD_TWIN_TEENSY40_HPP__
#define __BOARD_TWIN_TEENSY40_HPP__
#include "app_twin_teensy40.hpp"
#include "meridian_network_keys.hpp"
#include "meridian_parameter.hpp"
#include <board/meridian_board_twin_for_teensy40.hpp>

namespace meridian {

class BoardSetting : public board::MeridianBoardTwinForTeensy40 {
private:
  meridian::TestApp app;

public:
  BoardSetting() {}
  ~BoardSetting() {}
  /// @brief 区別させるための名前
  const char *get_name() override { return MERIDIAN_BOARD_NAME; }

protected:
  /// @brief ボードの初期化処理
  bool init() override {
    //////////////////////////////////////////////////////////
    // ログレベル変更
    //////////////////////////////////////////////////////////
    this->app.set_log_level_unit(OUTPUT_LOG_LEVEL::LEVEL_ALL);
    this->set_log_level_unit(OUTPUT_LOG_LEVEL::LEVEL_ALL);
    //////////////////////////////////////////////////////////
    // ボード設定
    //////////////////////////////////////////////////////////
    this->push_communication(this->diag);

    //////////////////////////////////////////////////////////
    // moduleの設定
    //////////////////////////////////////////////////////////
    // this->push_module(this->servo_pwm1);

    //////////////////////////////////////////////////////////
    // アプリケーションの設定
    //////////////////////////////////////////////////////////
    this->push_app(&this->app);
    return true;
  }
  /// @brief プラグインのセットアップ処理
  bool setup() override {
    // 初期化処理をここに記述
    return true;
  }
  /// @brief 入力処理
  bool loop_input(Meridim &a_meridim) override { return true; }
  /// @brief 処理の前処理
  bool loop_process_prepare(Meridim &a_meridim) override { return true; }
  /// @brief 処理の片付け
  bool loop_process_tidy_up(Meridim &a_meridim) override { return true; }
  /// @brief 出力処理
  bool loop_output(Meridim &a_meridim) override { return true; }
};

} // namespace meridian

#endif // __BOARD_TWIN_TEENSY40_HPP__
