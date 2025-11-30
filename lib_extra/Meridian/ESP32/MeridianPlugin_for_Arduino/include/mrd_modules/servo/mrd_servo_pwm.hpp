/**
 * @file mrd_servo_pwm.hpp
 * @brief
 * @version 0.1
 * @date 2025-07-18
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __MERIDIAN_MODULE_MRD_SERVO_PWM_HPP__
#define __MERIDIAN_MODULE_MRD_SERVO_PWM_HPP__
// ヘッダファイルの読み込み
#include <interface/i_mrd_driver.hpp>
#include <mrd_utils/mrd_string.hpp>
// ライブラリ導入
#include <Arduino.h>
#include <Servo.h>

namespace meridian {
namespace modules {

class MrdServoPwm : public IMrdDriver {
public:
  /// @brief カテゴリー名
  const char *get_category() override { return "Servo"; }
  /// @brief 区別させるための名前
  const char *get_name() override { return "PWM"; }
  MrdServoPwm(int a_index, uint8_t a_pin_num, uint16_t a_min_us, uint16_t a_max_us, int a_min_angle = 0, int a_max_angle = 100) {
    this->is_input = false; // 入力処理を無効にする
    this->is_output = true; // 出力処理を有効にする

    this->_index = a_index;
    this->_min_angle = a_min_angle;
    this->_max_angle = a_max_angle;

    this->_pin_num = a_pin_num;
    this->_min_us = a_min_us;
    this->_max_us = a_max_us;
  }

private:
  Servo _servo;
  int _index = -1;
  bool _setup_done = false; // セットアップが完了したかどうかのフラグ
  int _min_angle = 0;       // 最小角度
  int _max_angle = 180;     // 最大角度
  uint8_t _pin_num = 0;     // ピン番号
  int _min_us = 500;        // 最小PWM幅[us]
  int _max_us = 2400;       // 最大PWM幅[us]

  struct ServoData {
  public:
    bool is_valid = false; // 有効なサーボデータかどうか
    int min_angle = 0;     // 最小角度
    int max_angle = 180;   // 最大角度
  };
  ServoData _servo_data[MERIDIM_SERVO_NUM] = {}; // サーボデータ配列

protected:
  /// @brief プラグインの初期化
  bool setup() override {
    if (0 <= this->_pin_num) {
      // セットアップ処理を実装
      if (0 <= this->_servo.attach(this->_pin_num, this->_min_us, this->_max_us)) {
        this->_setup_done = true;
      }
    }
    return this->_setup_done;
  }
  /// @brief 入力処理の実処理
  bool input(Meridim &a_meridim) override {
    return true;
  }
  /// @brief 出力処理の実処理
  bool output(Meridim &a_meridim) override {
    bool result = true;
    for (int i = 0; i < MERIDIM_SERVO_NUM; i++) {
      if (i == this->_index) {
        int value = a_meridim.userdata.servo[i].value;
        if (this->_max_angle < value) {
          value = this->_max_angle;
        } else if (value < this->_min_angle) {
          value = this->_min_angle;
        }
        value = map(value, this->_min_angle, this->_max_angle, 0, 180);
        this->_servo.write(this->_pin_num, value);
        this->debug(mrd_format("Servo[%d] Value: %d -> %d", i, a_meridim.userdata.servo[i].value, value).c_str());
      }
    }
    return result;
  }
};

} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULE_MRD_SERVO_PWM_HPP__
