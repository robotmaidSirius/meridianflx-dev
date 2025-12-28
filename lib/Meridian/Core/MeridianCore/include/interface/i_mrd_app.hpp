/**
 * @file i_mrd_app.hpp
 * @brief プラグインのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_APP_I_MRD_APP_HPP__
#define __MERIDIAN_APP_I_MRD_APP_HPP__
// ヘッダファイルの読み込み
#include "Meridim.hpp"
#include "meridian_core_settings.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

namespace meridian {
namespace app {

/// @brief プラグインのためのインターフェイスクラス
class IMrdApp : public meridian::communication::MeridianDiagnosticUnit {
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  ///   実装時には、以下の関数をオーバーライドすること
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 仮想関数 - カテゴリー名
  virtual const char *get_category() { return "GENERAL"; }
  /// @brief 仮想関数 - 区別させるためのアプリケーション名前
  virtual const char *get_name() { return "Unknown"; }

protected:
  /// @brief 仮想関数 - アプリケーションの初期化
  virtual bool setup(Meridim &a_meridim) = 0;
  /// @brief 仮想関数 - Processの実処理
  virtual bool loop(Meridim &a_meridim) = 0;
  /// @brief 仮想関数 - アプリケーションの終了処理
  virtual bool closing(Meridim &a_meridim) { return true; };

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 有効/無効の取得
  bool is_enabled() const { return this->_enabled; }
  /// @brief 有効/無効の設定
  bool set_enabled(bool a_enabled) {
    this->_enabled = a_enabled;
    return this->_enabled;
  }
  /// @brief インターバルの設定
  void set_interval(int a_interval_us) {
    this->_interval_us = a_interval_us;
  }

  /// @brief 初期化を実行する
  bool begin(Meridim &a_meridim) {
    if (false == this->_is_initialized) {
      this->_is_initialized = this->setup(a_meridim);
      if (true == this->_is_initialized) {
        this->update(a_meridim, this->_interval_us);
      }
    }
    return this->_is_initialized;
  }

  /// @brief 処理を実行する
  bool update(Meridim &a_meridim, int a_elapsed_time_us) {
    bool result = true;
    if (true == this->_is_initialized) {
      if (true == this->has_reached_checkpoint(a_elapsed_time_us)) {
        if (true == this->_enabled) {
          result = this->loop(a_meridim);
        }
      }
    }
    return result;
  }
  /// @brief 終了処理を実行する
  bool end(Meridim &a_meridim) {
    this->_is_initialized = !this->closing(a_meridim);
    return true;
  }

  /// @brief チェックポイントに到達したか確認する
  bool has_reached_checkpoint(int a_elapsed_time_us) {
    this->_elapsed_time_us += a_elapsed_time_us;
    if ((this->_elapsed_time_us >= this->_interval_us) || (true == this->_trigger)) {
      this->_elapsed_time_us -= this->_interval_us;
      if (true == this->_trigger) {
        this->_trigger = false;
      }
      return true;
    }
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  bool _is_initialized = false;                   ///! 初期化フラグ
  bool _enabled = true;                           ///! 有効フラグ
  int _interval_us = IMrdApp_DEFAULT_INTERVAL_US; ///! タイマーの間隔(マイクロ秒)
  int _elapsed_time_us = 0;                       ///! 経過時間(マイクロ秒)
  bool _trigger = false;                          ///! 即時実行フラグ
};

} // namespace app
} // namespace meridian

using namespace meridian::app;

#endif // __MERIDIAN_APP_I_MRD_APP_HPP__
