/**
 * @file mrd_module_analog_in.hpp
 * @brief
 * @version 1.0.0
 * @date 2025-01-19
 *
 * @copyright Copyright (c) 2025-.
 *
 */
#ifndef __MERIDIAN_MODULE_MRD_ANALOG_IN_HPP__
#define __MERIDIAN_MODULE_MRD_ANALOG_IN_HPP__
// ライブラリ導入
#include <Arduino.h>
#include <interface/i_mrd_driver.hpp>

namespace meridian {
namespace modules {

class MrdAnalogIn : public IMrdDriver {
public:
  MrdAnalogIn(uint8_t a_pin, int a_user_data_index) {
    assert(-1 <= a_user_data_index && a_user_data_index < MERIDIM_USER_DATA_SIZE);
    this->_pin = a_pin;
    this->_index = a_user_data_index;
  }
  ~MrdAnalogIn() {
    // Do nothing
  }

public:
  bool setup() override {
    if (0xFF != this->_pin) {
      pinMode(this->_pin, ANALOG);
      return true;
    }
    return false;
  }
  int read() {
    return analogRead(this->_pin);
  }

  bool input(Meridim &a_meridim) override {
    // a_meridim.userdata.options[this->m_index] = this->read();
    return true;
  }

  bool output(Meridim &a_meridim) override {
    // Do nothing
    return true;
  }

private:
  uint8_t _pin = 0xFF;
  int _index = 0;
};

} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULE_MRD_ANALOG_IN_HPP__
