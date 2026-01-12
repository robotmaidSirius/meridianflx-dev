/**
 * @file app_lite_pico.hpp
 * @brief アプリケーションヘッダーファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __APP_LITE_PICO_HPP__
#define __APP_LITE_PICO_HPP__
#include <interface/i_mrd_app.hpp>

namespace meridian {

class TestApp : public app::IMrdApp {
public:
  /// @brief 区別させるための名前
  const char *get_name() override { return "MeridianLite for Raspberry Pi Pico"; }

  TestApp() {
    this->set_interval(10);  // 10msごとに処理
    this->set_enabled(true); // 有効化
  }
  ~TestApp() {}

protected:
  /// @brief 初期化を実行する
  bool setup(Meridim &a_meridim) override {
    // 初期化処理をここに記述
    this->info("<APP> TestApp::setup() called");
    return true;
  }
  /// @brief Processの実処理
  bool loop(Meridim &a_meridim) override {
    this->info("<APP> Application Loop Start");
    return true;
  }
  /// @brief 終了処理を実行する
  bool closing(Meridim &a_meridim) override {
    // 終了処理をここに記述
    return true;
  }
};

} // namespace meridian

#endif // __APP_LITE_PICO_HPP__
