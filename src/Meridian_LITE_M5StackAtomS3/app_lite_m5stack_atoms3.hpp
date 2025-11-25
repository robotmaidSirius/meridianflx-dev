/**
 * @file app_lite_m5stack_atoms3.hpp
 * @brief アプリケーションヘッダーファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __APP_LITE_M5STACK_ATOMS3_HPP__
#define __APP_LITE_M5STACK_ATOMS3_HPP__
#include <interface/i_mrd_app.hpp>

namespace meridian {

class TestApp : public app::IMeridianApp {
public:
  TestApp() {}
  ~TestApp() {}

protected:
  /// @brief 初期化を実行する
  bool setup() override {
    // 初期化処理をここに記述
    return true;
  }
  /// @brief 初期化を実行する
  bool loop() override {
    // 定期的な更新処理をここに記述
    return true;
  }
};

} // namespace meridian

#endif // __APP_LITE_M5STACK_ATOMS3_HPP__
