/**
 * @file mrd_pwm_pca9685.hpp
 * @brief
 * @version 0.1
 * @date 2025-08-12
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __MERIDIAN_MODULE_MRD_PWM_PCA9685_HPP__
#define __MERIDIAN_MODULE_MRD_PWM_PCA9685_HPP__
// ヘッダファイルの読み込み
#include <interface/i_mrd_driver.hpp>
// ライブラリ導入
#include <Arduino.h>
#include <Wire.h>

namespace meridian {
namespace modules {

class MrdPwmPca9685 : public IMrdDriver {
protected:
  /// @brief プラグインの初期化
  bool setup() override { return true; }
  /// @brief 入力処理の実処理
  bool input(Meridim &a_meridim) override { return true; }
  /// @brief 出力処理の実処理
  bool output(Meridim &a_meridim) override { return true; }

public:
  /// @brief カテゴリー名
  const char *get_category() override { return "PWM"; }
  /// @brief 区別させるための名前
  const char *get_name() override { return "PCA9685"; }

  MrdPwmPca9685(TwoWire *a_wire, uint8_t a_address = 0x40) : IMrdDriver(false, true) {
    // this->config_control(false, true); // 入力:Off/出力:On

    this->_wire = a_wire;
    this->_address = a_address;
  }

protected:
  const int REGISTER_MODE1 = 0x00;
  const int REGISTER_MODE2 = 0x01;
  const int REGISTER_SUBADDRESS1 = 0x02;
  const int REGISTER_SUBADDRESS2 = 0x03;
  const int REGISTER_SUBADDRESS3 = 0x04;
  const int REGISTER_ALLCALLADDRESS = 0x05;
  const int REGISTER_ALL_LED_ON_L = 0xFA;
  const int REGISTER_ALL_LED_ON_H = 0xFB;
  const int REGISTER_ALL_LED_OFF_L = 0xFC;
  const int REGISTER_ALL_LED_OFF_H = 0xFD;
  const int REGISTER_PRE_SCALE = 0xFE;
  const int REGISTER_TESTMODE = 0xFF;

  const int POS_MODE1_RESTART = 0x07;
  const int POS_MODE1_EXTCLC = 0x06;
  const int POS_MODE1_AUTOINCREMENT = 0x05;
  const int POS_MODE1_SLEEP = 0x04;
  const int POS_MODE1_SUB1 = 0x03;
  const int POS_MODE1_SUB2 = 0x02;
  const int POS_MODE1_SUB3 = 0x01;
  const int POS_MODE1_ALLCALL = 0x00;
  const int POS_MODE2_INVRT = 0x04;
  const int POS_MODE2_OCH = 0x03;
  const int POS_MODE2_OUTDRV = 0x02;
  const int POS_MODE2_OUTNE_1 = 0x01;
  const int POS_MODE2_OUTNE_0 = 0x00;

private:
  TwoWire *_wire;   // I2C通信のためのワイヤーオブ
  uint8_t _address; // PCA9685のI2Cアドレス
};

} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULE_MRD_PWM_PCA9685_HPP__
