/**
 * @file mrd_diagnostic_uart.hpp
 * @brief
 * @version 1.2.0
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025-.
 *
 */
#ifndef MRD_DIAGNOSTIC_UART_HPP
#define MRD_DIAGNOSTIC_UART_HPP

#include <Arduino.h>
#include <interface/i_mrd_communication_diagnostic.hpp>

namespace meridian {
namespace core {
namespace communication {

class MrdDiagnosticUart : public meridian::core::communication::IMeridianDiagnostic {
public:
  MrdDiagnosticUart(HardwareSerial *a_serial, uint32_t a_baudrate = 115200, OUTPUT_LOG_LEVEL a_level = OUTPUT_LOG_LEVEL::LEVEL_INFO) : IMeridianDiagnostic() {
    this->disable();
    this->_serial = a_serial;
    this->_baudrate = a_baudrate;
    this->set_level(a_level);
  }

public:
  const char *get_name() override { return "UART"; }
  bool setup() override {
    if (nullptr != this->_serial) {
      this->_serial->begin(this->_baudrate);
      this->enable();
      return true;
    }
    return false;
  }

  size_t message(OUTPUT_LOG_LEVEL level, const char *message) override {
    if (nullptr != this->_serial) {
      if (OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL == level) {
        return this->_serial->printf("%s", message);
      } else {
        return this->_serial->printf("[%s] %s\n", this->get_text_level(level), message);
      }
    }
    return 0;
  }

private:
  HardwareSerial *_serial;
  uint32_t _baudrate = 115200;
};

} // namespace communication
} // namespace core
} // namespace meridian

#endif // MRD_DIAGNOSTIC_UART_HPP
