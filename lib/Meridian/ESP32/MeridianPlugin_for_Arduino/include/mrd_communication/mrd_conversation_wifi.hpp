/**
 * @file mrd_conversation_wifi.hpp
 * @brief
 * @version 1.2.0
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025-.
 *
 */

#ifndef MRD_CONVERSATION_WIFI_HPP
#define MRD_CONVERSATION_WIFI_HPP

// ヘッダファイルの読み込み
#include "mrd_module/gpio/mrd_module_gpio_out.hpp"
#include <interface/i_mrd_communication_conversation.hpp>
#include <interface/i_mrd_communication_diagnostic.hpp>
#include <meridian_core.hpp>

// ライブラリ導入
#include <WiFi.h>
#include <WiFiUdp.h>

namespace meridian {
namespace core {
namespace communication {

class MrdConversationWifi : public IMeridianConversation {
public:
  MrdConversationWifi(MrdGpioOut *connect = nullptr, MrdGpioOut *signal = nullptr) : IMeridianConversation() {
    this->_gpio_connect = connect;
    this->_gpio_signal = signal;
    if (nullptr != this->_gpio_connect) {
      this->_gpio_connect->setup();
      this->_gpio_connect->write(0, true);
    }
    if (nullptr != this->_gpio_signal) {
      this->_gpio_signal->setup();
      this->_gpio_signal->write(0, true);
    }
  }

public:
  const char *get_name() override { return "Wifi"; }
  bool setup() override {
    return true;
  }

private:
  bool received(Meridim90 &a_meridim) {
    static int a_len = MERIDIM90_BYTE_LEN;
    if (this->a_udp.available() >= a_len) {
      if (this->a_udp.parsePacket() >= a_len) // データの受信バッファ確認
      {
        if (nullptr != this->_gpio_signal) {
          this->_gpio_signal->write(1, true);
        }

        byte a_meridim_array[a_len] = {0};
        this->a_udp.read(a_meridim_array, a_len); // データの受信
        // TODO: 実装する
        // meridian::core::execution::mrd_convert_Meridim90(a_meridim, a_meridim_array, a_len);

        if (nullptr != this->_gpio_signal) {
          this->_gpio_signal->write(0, true);
        }
        return true;
      }
    }
    return false; // バッファにデータがない
  }
  bool send(Meridim90 &a_meridim) {
    bool result = this->check_connect();
    if (result) {
      if (nullptr != this->_gpio_signal) {
        this->_gpio_signal->write(1, true);
      }
#if false
      // TODO: 処理を記載する
       meridian::core::execution::meridim_countup(a_meridim);
        uint8_t a_meridim_array[MERIDIM90_BYTE_LEN] = {0};
        meridian::core::execution::mrd_convert_array(a_meridim_array, MERIDIM90_BYTE_LEN, a_meridim);
      for (int i = 0; i < MrdConversationWifi::NUMBER_ALLOWED; i++) {
        if (0 != target[i].port) {
          this->a_udp.beginPacket(target[i].ip, target[i].port);  // UDPパケットの開始
          this->a_udp.write(a_meridim_array, MERIDIM90_BYTE_LEN); // データの書き込み
          this->a_udp.endPacket();                                // UDPパケットの終了
        }
      }
#endif
      if (nullptr != this->_gpio_signal) {
        this->_gpio_signal->write(0, true);
      }
    }
    return result;
  }

public:
  void set_diagnostic(IMeridianDiagnostic &ref) override {
    this->m_diag = &ref;
    if (nullptr != this->_gpio_connect) {
      this->_gpio_connect->set_diagnostic(ref);
    }
    if (nullptr != this->_gpio_signal) {
      this->_gpio_signal->set_diagnostic(ref);
    }
  }
  bool check_connect() {
    bool result = (WiFi.status() != WL_CONNECTED) ? false : true;
    if (nullptr != this->_gpio_connect) {
      this->_gpio_connect->write(result ? 1 : 0, true);
    }
    return result;
  }

  bool connect(const char *ssid, const char *password, int open_port = 22224) {
    this->_open_port = open_port;
    WiFi.disconnect(true, true); // 新しい接続のためにWiFi接続をリセット
    delay(100);
    WiFi.begin(ssid, password); // Wifiに接続
    int delay_ms = 50;
    int timeout_ms = 10 * (1000);
    int logging_time_ms = (500);

    if (nullptr != this->_gpio_connect) {
      this->_gpio_connect->write(0, true);
    }
    if (nullptr != this->_gpio_signal) {
      this->_gpio_signal->write(0, true);
    }
    bool flag_output_once = false;
    while (WiFi.status() != WL_CONNECTED) { // https://www.arduino.cc/en/Reference/WiFiStatus 戻り値一覧
      timeout_ms -= delay_ms;
      if (this->_output_log) {
        if (0 == timeout_ms % logging_time_ms) { // 0.5秒ごとに接続状況を出力
          this->m_diag->log(OUTPUT_LOG_LEVEL::LEVEL_INFO, ".");
          flag_output_once = true;
        }
      }
      delay(delay_ms);       // 接続が完了するまでループで待つ
      if (0 >= timeout_ms) { // 10秒でタイムアウト
        if (flag_output_once) {
          this->m_diag->log(OUTPUT_LOG_LEVEL::LEVEL_INFO, "\n");
        }
        this->m_diag->log(OUTPUT_LOG_LEVEL::LEVEL_ERROR, "Wifi init TIMEOUT.");
        return false;
      }
    }
    if (this->_output_log && flag_output_once) {
      this->m_diag->log(OUTPUT_LOG_LEVEL::LEVEL_INFO, "\n");
    }
    this->a_udp.setTimeout(this->_timeout_ms);
    uint8_t result = this->a_udp.begin(this->_open_port);
    if (0 != result) {
      if (nullptr != this->_gpio_connect) {
        this->_gpio_connect->write(1, true);
      }
      return true;
    }
    return false;
  }

  const char *get_ip_address() {
    return WiFi.localIP().toString().c_str();
  }
  bool add_target(const char *ip, uint16_t port) {
    for (int i = 0; i < MrdConversationWifi::NUMBER_ALLOWED; i++) {
      if (0 == target[i].port) {
        target[i].ip.fromString(ip);
        target[i].port = port;
        return true;
      }
    }
    return false;
  }
  bool clear_target() {
    for (int i = 0; i < MrdConversationWifi::NUMBER_ALLOWED; i++) {
      target[i].port = 0;
    }
    return true;
  }
  bool clear_target(int index) {
    if (0 <= index && index < MrdConversationWifi::NUMBER_ALLOWED) {
      target[index].port = 0;
      return true;
    }
    return false;
  }

private:
  int _open_port = 22224;
  MrdGpioOut *_gpio_connect = nullptr;
  MrdGpioOut *_gpio_signal = nullptr;

  WiFiUDP a_udp; // wifi設定
  struct target_send {
    IPAddress ip;
    uint16_t port = 0;
  };
  bool _output_log = true;
  int _timeout_ms = 5;

public:
  static const int NUMBER_ALLOWED = 1;
  target_send target[MrdConversationWifi::NUMBER_ALLOWED];
};

} // namespace communication
} // namespace core
} // namespace meridian

#endif // MRD_CONVERSATION_WIFI_HPP
