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

#include <interface/i_board.hpp>
#include <interface/i_mrd_communication_conversation.hpp>
#include <interface/i_mrd_communication_diagnostic.hpp>
#include <interface/i_mrd_plugin.hpp>

namespace meridian {
namespace board {

class MeridianBoardAtom : public IBoard {
public:
  struct Parameter {
    bool is_network_ap_mode = true;
    bool is_network_sta_mode = false;
    bool is_factory_mode = false;
  };
  struct Plugin {
  public:
    /// @brief 通信用のインターフェースと診断用のインターフェースをまとめた構造体
    struct mrd_communication {
    public:
      IMeridianConversation *con; ///< 通信用のインターフェース
      IMeridianDiagnostic *diag;  ///< 診断用のインターフェース
    };
    struct mrd_plugin {
    public:
      IMeridianPlugin *pad;
    };

  public:
    CallbackProcess Processing = nullptr;

  public:
    mrd_communication communication; ///< 通信用のインターフェース
    mrd_plugin plugin;               ///< プラグインのインターフェース
  };

public:
  MeridianBoardAtom() {}
  ~MeridianBoardAtom() {}

  bool Setup() {

    Plugin entity;

    this->_parameter = new Parameter();
    bool result = this->Setup(this->_plugin, this->_parameter);

    return this->_is_setup;
  }
  bool Setup(Plugin *a_plugin, Parameter *a_parameter) {
    bool result = false;
    if (nullptr != a_plugin) {
      this->_plugin = a_plugin;
    } else {
      this->_plugin = new Plugin();
    }
    if (nullptr != a_parameter) {
      this->_parameter = a_parameter;
    } else {
      this->_parameter = new Parameter();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    result = true;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    this->_is_setup = result;
    return this->_is_setup;
  }

public:
  bool Loop(Meridim90 &a_meridim) {
    bool result = false;
    // Meridim90 a_meridim;

    if (true == this->Input(a_meridim)) {
      // アプリ処理を記載する
      // CALLBACK

      result = this->Output(a_meridim);
    }
    return result;
  }

  bool Input(Meridim90 &a_meridim) { return true; }
  bool Processing(Meridim90 &a_meridim) { return true; }
  bool Output(Meridim90 a_meridim) { return true; }

private:
  Plugin *_plugin = nullptr;
  Parameter *_parameter = nullptr;
  bool _is_setup = false;
};
} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
