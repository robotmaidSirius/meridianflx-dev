/**
 * @file i_mrd_driver.hpp
 * @brief ドライバのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__
#define __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__

#include "Meridim.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

namespace meridian {
namespace modules {

/// @brief プラグインのためのインターフェイスクラス
class IMeridianDriver : public meridian::communication::MeridianDiagnosticUnit {
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief カテゴリー名
  virtual const char *get_category() { return "NotCategorized"; }
  /// @brief 区別させるための名前
  virtual const char *get_name() { return "Unknown"; }

protected:
  /// @brief プラグインの初期化
  virtual bool setup() = 0;
  /// @brief 仮想関数 - 入力処理処理の実処理
  virtual bool input(Meridim &a_meridim) = 0;
  /// @brief 仮想関数 - 出力処理の実処理
  virtual bool output(Meridim &a_meridim) = 0;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  void config_control(bool a_is_input, bool a_is_output) {
    this->is_input = a_is_input;
    this->is_output = a_is_output;
  }

  /// @brief プラグインの初期化
  bool begin() {
    if (false == this->_is_initialized) {
      this->_is_initialized = this->setup();
    }
    return this->_is_initialized;
  }
  /// @brief 仮想関数 - 入力処理の実処理
  bool reception(Meridim &a_meridim) {
    bool result = true;
    if (true == this->_is_initialized) {
      result = this->input(a_meridim);
    }
    return result;
  }
  /// @brief 仮想関数 - 出力処理の実処理
  bool transmission(Meridim &a_meridim) {
    bool result = true;
    if (true == this->_is_initialized) {
      result = this->output(a_meridim);
    }
    return result;
  }
  /// @brief 入力処理を実行するかを制御します。フラグがtrueの場合、入力処理を行います。
  bool is_input = false;

  /// @brief 出力処理を実行するかを制御します。フラグがtrueの場合、出力処理を行います。
  bool is_output = false; ///! 出力フラグ

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  bool _is_initialized = false; ///! 初期化フラグ
};

} // namespace modules
} // namespace meridian

using namespace meridian::modules;

#endif // __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__
