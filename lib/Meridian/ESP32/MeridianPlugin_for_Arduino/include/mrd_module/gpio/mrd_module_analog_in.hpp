/**
 * @file mrd_module_analog_in.hpp
 * @brief
 * @version 1.2.0
 * @date 2025-01-19
 *
 * @copyright Copyright (c) 2025-.
 *
 */
#ifndef __MRD_MODULE_ANALOG_HPP__
#define __MRD_MODULE_ANALOG_HPP__

// ライブラリ導入
#include <Arduino.h>
#include <interface/i_mrd_plugin.hpp>

namespace meridian {
namespace modules {
namespace plugin {

class MrdAnalogIn : public IMeridianPlugin {
public:
  MrdAnalogIn(uint8_t a_pin, int a_user_data_index) {
    assert(-1 <= a_user_data_index && a_user_data_index < MERIDIM90_USER_DATA_SIZE);
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
  int read() override {
    return analogRead(this->m_pin);
  }

  bool input(Meridim90 &a_meridim) override {
    // a_meridim.userdata.options[this->m_index] = this->read();
    return true;
  }

  bool output(Meridim90 &a_meridim) override {
    // Do nothing
    return true;
  }

private:
  uint8_t _pin = 0xFF;
  int _index = 0;
};

} // namespace plugin
} // namespace modules
} // namespace meridian

#endif // __MRD_MODULE_ANALOG_HPP__
