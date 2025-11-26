/**
 * @file board_lite_m5stack_atoms3.hpp
 * @brief Meridian LITE [Board: M5Stack-AtomS3]のボード設定ファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __BOARD_LITE_M5STACK_ATOMS3_HPP__
#define __BOARD_LITE_M5STACK_ATOMS3_HPP__
#include "app_lite_m5stack_atoms3.hpp"
#include "meridian_network_keys.hpp"
#include "meridian_parameter.hpp"
#include <board/meridian_board_atom_s3.hpp>

namespace meridian {

class BoardSetting : public board::MeridianBoardOnAtomS3 {
public:
  BoardSetting() {}
  ~BoardSetting() {}

protected:
  /// @brief 初期化を実行する
  bool setup() override {
    // 初期化処理をここに記述
    return true;
  }
  /// @brief 入力処理
  bool loop_input(Meridim &a_meridim) override { return true; }
  /// @brief 処理の前処理
  bool loop_process_prepare(Meridim &a_meridim) override { return true; }
  /// @brief 処理の片付け
  bool loop_process_tidy_up(Meridim &a_meridim) override { return true; }
  /// @brief 出力処理
  bool loop_output(Meridim &a_meridim) override { return true; }
};

} // namespace meridian

#endif // __BOARD_LITE_M5STACK_ATOMS3_HPP__
