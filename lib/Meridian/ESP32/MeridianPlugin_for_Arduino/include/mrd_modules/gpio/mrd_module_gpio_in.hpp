/**
 * @file mrd_module_gpio_in.hpp
 * @brief
 * @version 1.0.0
 * @date 2025-01-19
 *
 * @copyright Copyright (c) 2025-.
 *
 */
#ifndef __MERIDIAN_MODULE_MRD_GPIO_IN_HPP__
#define __MERIDIAN_MODULE_MRD_GPIO_IN_HPP__
// ライブラリ導入
#include <Arduino.h>
#include <interface/i_mrd_driver.hpp>

namespace meridian {
namespace modules {

class MrdGpioIN : public IMrdDriver {

public:
  MrdGpioIN(uint8_t a_pin, int a_user_data_index) {
    assert(-1 <= a_user_data_index && a_user_data_index < MERIDIM_USER_DATA_SIZE);

    this->_index = a_pin;
    this->_index = a_user_data_index;
  }
  ~MrdGpioIN() {
    // Do nothing
  }

public:
  bool setup() override {
    if (0xFF != this->_pin) {
      pinMode(this->_pin, INPUT_PULLUP);
      return true;
    }
    return false;
  }

  int read() {
    return digitalRead(this->_pin);
  }

  bool input(Meridim &a_meridim) override {
#if false
    // TODO: ビットで格納する場合のロジック
    if (0 < this->read()) {
      a_meridim.userdata.options[this->m_index] = this->_pos | a_meridim.userdata.options[this->_index];
    } else {
      a_meridim.userdata.options[this->m_index] = ~(this->_pos) & a_meridim.userdata.options[this->_index];
    }
#else
    // TODO: クラスを定義する
    // a_meridim.userdata.options[this->_index] = this->read();
#endif

    return true;
  }

  bool output(Meridim &a_meridim) override {
    // Do nothing
    return true;
  }

private:
  uint8_t _pin = 0xFF;
  int _index = 0;

public:
};

} // namespace modules
} // namespace meridian

#endif // __MRD_MODULE_MRD_GPIO_IN_HPP__
