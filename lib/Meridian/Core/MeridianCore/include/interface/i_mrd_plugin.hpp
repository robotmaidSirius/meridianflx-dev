/**
 * @file i_mrd_plugin.hpp
 * @brief プラグインのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
#define __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__

#include "Meridim90.hpp"
#include "interface/i_mrd_communication_diagnostic_append.hpp"

namespace meridian {
namespace modules {
namespace plugin {

using namespace meridian::core::communication;

/// @brief プラグインのためのインターフェイスクラス
class IMeridianPlugin : public IMeridianDiagnosticAppend {
public:
  /// @brief コンストラクタ
  /// @param a_is_input 入力フラグ
  /// @param a_is_processing 処理フラグ
  /// @param a_is_output 出力フラグ
  IMeridianPlugin(bool a_is_input = false, bool a_is_processing = false, bool a_is_output = false) {
    this->is_input = a_is_input;
    this->is_output = a_is_processing;
    this->is_processing = a_is_output;
  }

public:
  /// @brief 区別させるための名前
  virtual const char *get_name() { return "Unknow"; }
  /// @brief プラグインの初期化
  virtual bool setup() { return true; }
  /// @brief 仮想関数 - 入力処理処理の実処理
  virtual bool input(Meridim90 &a_meridim) { return true; }
  /// @brief 仮想関数 - Processの実処理
  virtual bool processing(Meridim90 &a_meridim) { return true; }
  /// @brief 仮想関数 - 出力処理の実処理
  virtual bool output(Meridim90 &a_meridim) { return true; }

public:
  bool is_input = false;      ///! 入力フラグ
  bool is_output = false;     ///! 出力フラグ
  bool is_processing = false; ///! 処理フラグ

protected:
  /// @brief 入力処理を実行するかを制御します。フラグがtrueの場合、入力処理を行います。
  /// @param a_flag 入力処理を実行するか
  void set_contorl_input(bool a_flag) {
    this->is_input = a_flag;
  }
  /// @brief 出力処理を実行するかを制御します。フラグがtrueの場合、出力処理を行います。
  /// @param a_flag 出力処理を実行するか
  void set_contorl_output(bool a_flag) {
    this->is_output = a_flag;
  }
  /// @brief Processを実行するかを制御します。フラグがtrueの場合、処理を行います。
  /// @param a_flag 処理を実行するか
  void set_contorl_processing(bool a_flag) {
    this->is_processing = a_flag;
  }

#if false
public:
  class Status {
  public:
    Status(int a_option_max = 0) {
      this->OPTION_SIZE = a_option_max;

      // TODO: 配列を動的に確保する
      this->option = new bool[this->OPTION_SIZE];
      for (int i = 0; i < this->OPTION_SIZE; i++) {
        this->option[i] = false;
      }
    }

    int OPTION_SIZE = 0;
    bool initalized = false;
    bool setup = false;
    bool *option;
  };

public:
  void get_status(Status &a_state) {
    a_state.initalized = this->state.initalized;
    a_state.setup = this->state.setup;
    for (int i = 0; i < this->state.OPTION_SIZE; i++) {
      a_state.option[i] = this->state.option[i];
    }
  }

protected:
  Status state;
#endif
};

} // namespace plugin
} // namespace modules
} // namespace meridian

using namespace meridian::modules::plugin;

#endif // __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
