/**
 * @file meridian_board_atom_.hpp
 * @brief "Meridian Board on M5Atom"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__

#include "board/pins/meridian_board_atom_pins.hpp"
#include "board/setting/meridian_board_atom_setting.hpp"
#include "mrd_util_for_arduino.hpp"

#include <interface/i_board.hpp>
#include <mrd_module/mrd_timer.hpp>

/// @brief バージョン情報の定義
#define MERIDIAN_VERSION BUILD_BOARD_NAME " ver." BUILD_VERSION

namespace meridian {
namespace board {

class MeridianBoardAtom : public IBoard {
public:
  class Parameter {
  public:
    bool is_network_ap_mode = true;
    bool is_network_sta_mode = false;
    bool is_factory_mode = false;
    int delay_time_ms = 10;
  };

public:
  typedef bool (*CallbackProcess)(Meridim90 &a_meridim);
  Parameter parameter;
  CallbackProcess event_process = nullptr;

public:
  MeridianBoardAtom() {}
  ~MeridianBoardAtom() {}

  bool setup() {
    bool result = false;
    try {
      ////////////////////////////////////////////////////////////////////////////////////////////////
      if (nullptr == this->diag) {
        this->diag = new IMeridianDiagnostic();
      }

      this->con->set_diagnostic(*this->diag);
      this->diag->setup();
      this->info("==========================================");
      this->info("Hello M5Stack Atom!");
      this->info("Version: " MERIDIAN_VERSION);
      this->info(mrd_format("Formatted Build Time: %s", GetTimeString(BUILD_TIME).c_str()).c_str());

      ////////////////////////////////////////////////////////////////////////////////////////////////
      meridian::modules::mrd_timer_setup(this->parameter.delay_time_ms, SETTING_DEFAULT_TIMER_SECTION_US, SETTING_DEFAULT_TIMER_DELAY_RATE);
      result = true;
    } catch (...) {
      this->error("Exception occurred during setup.");
      result = false;
    }
    this->_is_setup = result;
    return this->_is_setup;
  }

public:
  /// @brief 入力処理
  bool input(Meridim90 &a_meridim) override { return true; };
  /// @brief 処理の前処理
  bool process_prepare(Meridim90 &a_meridim) override { return true; };
  /// @brief 処理の実処理
  bool processing(Meridim90 &a_meridim) override { return true; };
  /// @brief 処理の片付け
  bool process_tidy_up(Meridim90 &a_meridim) override { return true; };
  /// @brief 出力処理
  bool output(Meridim90 &a_meridim) override { return true; };

  /// @brief ループ時の待機処理
  void waiting() override {
    meridian::modules::mrd_timer_delay(SETTING_DEFAULT_TIMER_SECTION_US);
  }

private:
  bool _is_setup = false;
};

} // namespace board
} // namespace meridian

#undef MERIDIAN_VERSION

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
