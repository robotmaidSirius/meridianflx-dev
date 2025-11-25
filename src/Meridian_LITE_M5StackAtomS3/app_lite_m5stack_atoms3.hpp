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
  /// @brief 区別させるための名前
  const char *get_name() override { return "MeridianLite for M5Stack Atom S3"; }

  TestApp() {
    this->set_interval(10);  // 10msごとに処理
    this->set_enabled(true); // 有効化
  }
  ~TestApp() {}

protected:
  /// @brief 初期化を実行する
  bool setup(Meridim &a_meridim) override {
    // 初期化処理をここに記述
    return true;
  }
  /// @brief Processの実処理
  bool loop(Meridim &a_meridim) override {
    // 定期的な更新処理をここに記述
    return true;
  }
  /// @brief 終了処理を実行する
  bool closing(Meridim &a_meridim) override {
    // 終了処理をここに記述
    return true;
  }
};

} // namespace meridian

#endif // __APP_LITE_M5STACK_ATOMS3_HPP__
