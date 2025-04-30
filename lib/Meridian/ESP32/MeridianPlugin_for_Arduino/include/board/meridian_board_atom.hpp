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
#include <interface/i_mrd_communication_conversation.hpp>
#include <interface/i_mrd_communication_diagnostic.hpp>
#include <interface/i_mrd_plugin.hpp>

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
  };
  class Plugin {
  public:
    IMeridianConversation *con = nullptr; ///! 通信用のインターフェース
    IMeridianDiagnostic *diag = nullptr;  ///! 診断用のインターフェース
    IMeridianPlugin *pad = nullptr;
  };

public:
  Plugin plugin;
  Parameter parameter;
  CallbackProcess event_process = nullptr;

public:
  MeridianBoardAtom() {}
  ~MeridianBoardAtom() {}

  bool setup() override {
    bool result = false;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    result = true;
    if (nullptr == this->plugin.diag) {
      this->plugin.diag = new IMeridianDiagnostic();
    }

    this->plugin.con->set_diagnostic(*this->plugin.diag);
    this->plugin.diag->setup();
    this->plugin.diag->log(this->_level, "==========================================");
    this->plugin.diag->log(this->_level, "Hello M5Stack Atom!");
    this->plugin.diag->log(this->_level, "Version: " MERIDIAN_VERSION);
    this->plugin.diag->log(this->_level, "Formatted Build Time: %s", GetTimeString(BUILD_TIME).c_str());

    ////////////////////////////////////////////////////////////////////////////////////////////////
    this->_is_setup = result;
    return this->_is_setup;
  }

public:
  bool input(Meridim90 &a_meridim) override {
    return true;
  }
  bool processing(Meridim90 &a_meridim) override {
    return true;
  }
  bool output(Meridim90 a_meridim) override {
    return true;
  }

private:
  bool _is_setup = false;
  OUTPUT_LOG_LEVEL _level = OUTPUT_LOG_LEVEL::LEVEL_INFO;
};
} // namespace board
} // namespace meridian

#undef MERIDIAN_VERSION

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
