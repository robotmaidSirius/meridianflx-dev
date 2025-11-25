/**
 * @file board_lite_m5stack_atomlite.hpp
 * @brief Meridian LITE [Board: M5Stack-AtomLite]のボード設定ファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __BOARD_LITE_M5STACK_ATOMLITE_HPP__
#define __BOARD_LITE_M5STACK_ATOMLITE_HPP__
#include "app_lite_m5stack_atomlite.hpp"
#include "meridian_network_keys.hpp"
#include "meridian_parameter.hpp"
#include <board/meridian_board_atom_lite.hpp>

namespace meridian {

class BoardSetting : public board::MeridianBoardOnAtomLite {
public:
  BoardSetting() {}
  ~BoardSetting() {}

  /// @brief 区別させるための名前
  // const char *get_name() override { return "BoardSetting"; }
protected:
  /// @brief プラグインの初期化
  bool setup() override {
    return true;
  }

  /// @brief Processの実処理
  bool loop() override {
    return true;
  }
};

} // namespace meridian

#endif // __BOARD_LITE_M5STACK_ATOMLITE_HPP__
