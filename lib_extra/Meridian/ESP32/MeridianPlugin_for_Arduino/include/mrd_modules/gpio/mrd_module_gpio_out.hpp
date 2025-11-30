/**
 * @file mrd_module_gpio_out.hpp
 * @brief
 * @version 1.0.0
 * @date 2025-01-20
 *
 * @copyright Copyright (c) 2025-.
 *
 */
#ifndef __MERIDIAN_MODULE_MRD_GPIO_OUT_HPP__
#define __MERIDIAN_MODULE_MRD_GPIO_OUT_HPP__
// ヘッダファイルの読み込み
#include <interface/i_mrd_driver.hpp>
// ライブラリ導入
#include <Arduino.h>

namespace meridian {
namespace modules {

class MrdGpioOut : public IMrdDriver {
public:
  enum GPIO_FUNCTIONS {
    GPIO_OUTPUT = OUTPUT,
    GPIO_PULLUP = PULLUP,
    GPIO_INPUT_PULLUP = INPUT_PULLUP,
    GPIO_PULLDOWN = PULLDOWN,
    GPIO_INPUT_PULLDOWN = INPUT_PULLDOWN,
    GPIO_OPEN_DRAIN = OPEN_DRAIN,
    GPIO_OUTPUT_OPEN_DRAIN = OUTPUT_OPEN_DRAIN
  };

public:
  MrdGpioOut(uint8_t a_pin, int a_user_data_index, GPIO_FUNCTIONS a_func = GPIO_FUNCTIONS::GPIO_OUTPUT) {
    assert(-1 <= a_user_data_index && a_user_data_index < MERIDIM_USER_DATA_SIZE);

    this->_pin = a_pin;
    this->_index = a_user_data_index;
    this->_func = a_func;
  }
  ~MrdGpioOut() {
    // Do nothing
  }

public:
  bool setup() override {
    if (0xFF != this->_pin) {
      pinMode(this->_pin, OUTPUT);
      return true;
    }
    return false;
  }

  bool write(int value, bool flag) {
    digitalWrite(this->_pin, flag ? HIGH : LOW);
    return true;
  }

  bool input(Meridim &a_meridim) override {
    // Do nothing
    return true;
  }

  bool output(Meridim &a_meridim) override {
#if false
    // TODO: ビットで格納する場合のロジック
    if (this->m_output) {
      if (this->m_flag) {
        a_meridim.userdata.options[this->m_index] = this->m_pos | a_meridim.userdata.options[this->m_index];
      } else {
        a_meridim.userdata.options[this->m_index] = ~(this->m_pos) & a_meridim.userdata.options[this->m_index];
      }
    }
#else
    // TODO: クラスを定義する
    // this->write(this->_pin, a_meridim.userdata.options[this->_index]);
#endif
    return true;
  }

private:
  uint8_t _pin = 0xFF;
  int _index = -1;
  GPIO_FUNCTIONS _func = GPIO_FUNCTIONS::GPIO_OUTPUT;
};

} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULE_MRD_GPIO_OUT_HPP__
