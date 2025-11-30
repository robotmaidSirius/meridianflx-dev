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
// ヘッダファイルの読み込み
#include "Meridim.hpp"
#include "meridian_core_settings.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

namespace meridian {
namespace modules {

/// @brief プラグインのためのインターフェイスクラス
class IMrdDriver : public meridian::communication::MeridianDiagnosticUnit {
public:
  IMrdDriver(bool a_is_input, bool a_is_output) {
    this->config_control(a_is_input, a_is_output);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 仮想関数 - カテゴリー名
  virtual const char *get_category() { return "NotCategorized"; }
  /// @brief 仮想関数 - 区別させるための名前
  virtual const char *get_name() { return "Unknown"; }

  /// @brief インターバルの設定
  void set_interval(int a_interval_us) {
    this->_interval_reception_us = a_interval_us;
    this->_interval_transmission_us = a_interval_us;
  }
  void set_interval_input(int a_interval_us) {
    this->_interval_reception_us = a_interval_us;
  }
  void set_interval_output(int a_interval_us) {
    this->_interval_transmission_us = a_interval_us;
  }

protected:
  /// @brief 仮想関数 - プラグインの初期化
  virtual bool setup() = 0;
  /// @brief 仮想関数 - 入力処理処理の実処理
  virtual bool input(Meridim &a_meridim) = 0;
  /// @brief 仮想関数 - 出力処理の実処理
  virtual bool output(Meridim &a_meridim) = 0;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  //// @brief 入力処理と出力処理を実行するかを制御します。
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
      if (true == this->_is_running_reception) {
        if (true == this->is_input) {
          result = this->input(a_meridim);
        }
      }
    }
    return result;
  }
  /// @brief 仮想関数 - 出力処理の実処理
  bool transmission(Meridim &a_meridim) {
    bool result = true;
    if (true == this->_is_initialized) {
      if (true == this->_is_running_transmission) {
        if (true == this->is_output) {
          result = this->output(a_meridim);
        }
      }
    }
    return result;
  }
  /// @brief チェックポイントに到達したか確認する
  bool has_reached_checkpoint(int a_elapsed_time_us) {
    this->_elapsed_time_reception_us += a_elapsed_time_us;
    this->_elapsed_time_transmission_us += a_elapsed_time_us;
    if (this->_elapsed_time_reception_us >= this->_interval_reception_us) {
      this->_elapsed_time_reception_us -= this->_interval_reception_us;
      this->_is_running_reception = true;
    } else {
      this->_is_running_reception = false;
    }
    if (this->_elapsed_time_transmission_us >= this->_interval_transmission_us) {
      this->_elapsed_time_transmission_us -= this->_interval_transmission_us;
      this->_is_running_transmission = true;
    } else {
      this->_is_running_transmission = false;
    }
    return this->_is_running_transmission && this->_is_running_reception;
  }

  /// @brief 入力処理を実行するかを制御します。フラグがtrueの場合、入力処理を行います。
  bool is_input = false;
  /// @brief 出力処理を実行するかを制御します。フラグがtrueの場合、出力処理を行います。
  bool is_output = false;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  bool _is_initialized = false;                                   ///! 初期化フラグ
  bool _is_running_reception = false;                             ///! 実行中フラグ
  int _interval_reception_us = IMrdDriver_DEFAULT_INTERVAL_US;    ///! タイマーの間隔(マイクロ秒)
  int _elapsed_time_reception_us = 0;                             ///! 経過時間(マイクロ秒)
  bool _is_running_transmission = false;                          ///! 実行中フラグ
  int _interval_transmission_us = IMrdDriver_DEFAULT_INTERVAL_US; ///! タイマーの間隔(マイクロ秒)
  int _elapsed_time_transmission_us = 0;                          ///! 経過時間(マイクロ秒)
};

} // namespace modules
} // namespace meridian

using namespace meridian::modules;

#endif // __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__
