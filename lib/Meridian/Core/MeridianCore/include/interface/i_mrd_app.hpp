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

#include "Meridim.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

namespace meridian {
namespace app {

/// @brief プラグインのためのインターフェイスクラス
class IMeridianApp : public meridian::communication::MeridianDiagnosticUnit {
protected:
  /// @brief 初期化を実行する
  /// 関数をOverrideして、処理を記述
  virtual bool setup() {
    return true;
  }
  /// @brief 定期処理を実行する
  /// 関数をOverrideして、処理を記述
  virtual bool loop() {
    return true;
  }
};

} // namespace app
} // namespace meridian

using namespace meridian::app;

#endif // __MERIDIAN_APP_I_MRD_APP_HPP__
