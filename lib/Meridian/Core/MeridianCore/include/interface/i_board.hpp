/**
 * @file i_board.hpp
 * @brief ボードのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_I_BOARD_HPP__
#define __MERIDIAN_BOARD_I_BOARD_HPP__

#include "Meridim90.hpp"
#include "i_mrd_communication_conversation.hpp"
#include "i_mrd_plugin.hpp"
#include <list>

namespace meridian {
namespace board {

/// @brief ボードのインターフェイスクラス
/// @note Meridianのボードはこのクラスを継承して実装すること。
class IBoard : public IMeridianDiagnosticAppend {
public:
  /// @brief 準備完了フラグ
  virtual bool begin() { return true; }

protected:
  /// @brief ループ時の待機処理。ボードによってTimerの実装が異なるため、継承先で実装すること。
  virtual void waiting() = 0;

  /// @brief 入力処理
  virtual bool input(Meridim90 &a_meridim) { return true; };
  /// @brief 処理の前処理
  virtual bool process_prepare(Meridim90 &a_meridim) { return true; };
  /// @brief 処理の実処理
  virtual bool processing(Meridim90 &a_meridim) { return true; };
  /// @brief 処理の片付け
  virtual bool process_tidy_up(Meridim90 &a_meridim) { return true; };
  /// @brief 出力処理
  virtual bool output(Meridim90 &a_meridim) { return true; };

public:
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// モジュールの登録
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief communicationを登録する
  void push_communication(IMeridianConversation *a_con, IMeridianDiagnostic *a_diag) {
    this->diag = a_diag;
    this->con = a_con;
    if (nullptr != this->diag) {
      this->con->set_diagnostic(*this->diag);
    }
    for (auto &module : this->modules) {
      if (nullptr != module) {
        module->set_diagnostic(*this->diag);
      }
    }
  }
  /// @brief モジュールを登録します。
  bool push_module(IMeridianPlugin *a_plugin) {
    if (nullptr != a_plugin) {
      if (nullptr != this->diag) {
        a_plugin->set_diagnostic(*this->diag);
      }
      this->modules.push_back(a_plugin);
      return true;
    }
    return false;
  }

  /// @brief 処理を実行する
  /// @param a_meridim Meridim90の構造体
  /// @return true:正常終了, false:異常終了
  /// @note 実行順番は以下の通り
  /// * Input
  ///   01. [Conversation]受信処理
  ///   02. [継承先クラスの実装]input関数の処理
  ///   03. [Modules]モジュールの入力処理
  /// * Process
  ///   04. [継承先クラスの実装]Processの前処理
  ///   05. [Modules]モジュールの処理
  ///   06. [継承先クラスの実装]Processの実処理
  ///   07. [継承先クラスの実装]Processの片付け
  /// * output
  ///   08. [継承先クラスの実装]output関数の出力処理
  ///   09. [Modules]モジュールの出力処理
  ///   10. [Conversation]送信処理
  bool loop(Meridim90 &a_meridim) {
    bool result = this->_is_initialized;

    // 受信処理
    if (true == result) {
      if (nullptr != this->con) {
        result = this->con->received(a_meridim);
      }
    }

    // 継承されたinput関数の処理
    if (true == result) {
      result = this->input(a_meridim);
    }

    // 登録されたモジュールの入力処理
    if (true == result) {
      for (auto &module : this->modules) {
        if (nullptr != module) {
          if (true == module->is_input) {
            result &= module->input(a_meridim);
          }
        }
      }
    }

    // 継承されたProcessの前処理
    if (true == result) {
      result = this->process_prepare(a_meridim);
    }

    // 登録されたモジュールの処理
    if (true == result) {
      for (auto &module : this->modules) {
        if (nullptr != module) {
          if (true == module->is_processing) {
            result &= module->processing(a_meridim);
          }
        }
      }
    }

    // 継承されたProcessの実処理
    if (true == result) {
      result = this->processing(a_meridim);
    }

    // 継承されたProcessの片付け
    if (true == result) {
      result = this->process_tidy_up(a_meridim);
    }

    // 継承されたoutput関数の出力処理
    if (true == result) {
      result = this->output(a_meridim);
    }

    // モジュールの出力処理
    if (true == result) {
      for (auto &module : this->modules) {
        if (nullptr != module) {
          if (true == module->is_output) {
            result &= module->output(a_meridim);
          }
        }
      }
    }

    // 送信処理
    if (true == result) {
      if (nullptr != this->con) {
        result = this->con->send(a_meridim);
      }
    }
    // 待機処理
    this->waiting();
    return result;
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////

protected:
  bool _is_initialized = false;         ///! 初期化フラグ
  IMeridianConversation *con = nullptr; ///! 通信用のインターフェース
  std::list<IMeridianPlugin *> modules; ///! プラグインのインターフェース
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_I_BOARD_HPP__
