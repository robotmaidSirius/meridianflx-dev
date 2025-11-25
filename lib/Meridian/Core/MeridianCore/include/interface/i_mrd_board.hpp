/**
 * @file i_mrd_board.hpp
 * @brief ボードのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_I_MRD_BOARD_HPP__
#define __MERIDIAN_BOARD_I_MRD_BOARD_HPP__

#include "Meridim.hpp"
#include "interface/i_mrd_app.hpp"
#include "interface/i_mrd_conversation.hpp"
#include "interface/i_mrd_driver.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

#include <list>

namespace meridian {
namespace board {

using namespace meridian::app;
using namespace meridian::modules;
using namespace meridian::communication;

/// @brief ボードのインターフェイスクラス
/// @note Meridianのボードはこのクラスを継承して実装すること。
class IMrdBoard : public meridian::communication::MeridianDiagnosticUnit {

public:
  bool begin() {
    return this->setup();
  }
  bool update() {
    bool result = this->loop();
    this->waiting();
    return result;
  }

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
  /// @brief ループ時の待機処理
  virtual void waiting() = 0;
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_I_MRD_BOARD_HPP__
