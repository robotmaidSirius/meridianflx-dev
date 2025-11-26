/**
 * @file app_lite_m5stack_atomlite.hpp
 * @brief アプリケーションヘッダーファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __APP_LITE_M5STACK_ATOMLITE_HPP__
#define __APP_LITE_M5STACK_ATOMLITE_HPP__
#include <interface/i_mrd_app.hpp>

namespace meridian {

class TestApp : public app::IMeridianApp {
public:
  /// @brief 区別させるための名前
  const char *get_name() override { return "MeridianLite for M5Stack Atom Lite"; }

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
#if 0
    this->set_pwm(a_meridim); // サーボのPWM設定を行う

    this->info(mrd_format("============== esp_get_free_heap_size() : %6d", esp_get_free_heap_size()).c_str());
    this->info(mrd_format("MC:%3d,Seq:0x%04X", a_meridim.master_command, a_meridim.sequential).c_str());
    this->info(mrd_format("  acc:%3d,%3d,%3d", a_meridim.input_data.accelerator.x, a_meridim.input_data.accelerator.y, a_meridim.input_data.accelerator.z).c_str());
    this->info(mrd_format("  gyr:%3d,%3d,%3d", a_meridim.input_data.gyroscope.x, a_meridim.input_data.gyroscope.y, a_meridim.input_data.gyroscope.z).c_str());
    this->info(mrd_format("  mag:%3d,%3d,%3d", a_meridim.input_data.magnetometer.x, a_meridim.input_data.magnetometer.y, a_meridim.input_data.magnetometer.z).c_str());
    this->info(mrd_format("  temp:%3d", a_meridim.input_data.temperature).c_str());
    this->info(mrd_format("  dmp:%3d,%3d,%3d", a_meridim.input_data.dmp.roll, a_meridim.input_data.dmp.pitch, a_meridim.input_data.dmp.yaw).c_str());
    this->info(mrd_format("  control:0x%04X", a_meridim.input_data.control.buttons).c_str());
    this->info(mrd_format("  stick_l:%3d,stick_r:%3d", a_meridim.input_data.control.stick_l, a_meridim.input_data.control.stick_r).c_str());
    this->info(mrd_format("  analog_l:%3d,analog_r:%3d", a_meridim.input_data.control.analog_l, a_meridim.input_data.control.analog_r).c_str());
    this->info(mrd_format("  motion_frames:%d,stop_frames_ms:%d", a_meridim.userdata.motion_frames, a_meridim.userdata.stop_frames_ms).c_str());

    for (int i = 0; i < MERIDIM_SERVO_NUM; i++) {
      if (0 != a_meridim.userdata.servo[i].id) {
        this->info(mrd_format("  servo[%d]:id[%3d]cmd[%3d]val[%3d]", i, a_meridim.userdata.servo[i].id, a_meridim.userdata.servo[i].cmd, a_meridim.userdata.servo[i].value).c_str());
      }
    }
    String buffer = "";
    for (int i = 0; i < MERIDIM_USER_DATA_SIZE; i++) {
      buffer += mrd_format("[%d]%d ", i, a_meridim.userdata.options[i]).c_str();
    }
    this->info(mrd_format("  options:%s", buffer.c_str()).c_str());
    this->info(mrd_format("  err:0x%04X,checksum:0x%04X", a_meridim.err, a_meridim.checksum).c_str());

#endif
    return true;
  }
  /// @brief 終了処理を実行する
  bool closing(Meridim &a_meridim) override {
    // 終了処理をここに記述
    return true;
  }
};

} // namespace meridian

#endif // __APP_LITE_M5STACK_ATOMLITE_HPP__
