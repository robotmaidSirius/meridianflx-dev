/**
 * @file mrd_con_and_diag_wifi.hpp
 * @brief
 * @version 1.2.0
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025-.
 *
 */
// TODO: APモードの対応とSTAの対応を行う
// TODO: STAは複数の設定から呼び出せると良いかも
#ifndef __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_AND_DIAGNOSTIC_WIFI_HPP__
#define __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_AND_DIAGNOSTIC_WIFI_HPP__
// ヘッダファイルの読み込み
#include "mrd_modules/gpio/mrd_module_gpio_out.hpp"
#include "mrd_utils/mrd_string.hpp"
#include <interface/i_mrd_conversation.hpp>
#include <interface/i_mrd_diagnostic.hpp>
#include <meridian_core.hpp>

// ライブラリ導入
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

namespace meridian {
namespace communication {

class MrdConversationAndDiagnosticWifi : public IMrdConversation, public IMrdDiagnostic {
public:
  /// @brief コンストラクタ
  MrdConversationAndDiagnosticWifi(HardwareSerial *a_serial = nullptr, uint32_t a_baudrate = 115200) : IMrdConversation(), IMrdDiagnostic() {
    this->disable();
    this->_serial = a_serial;
    this->_baudrate = a_baudrate;
  }

public:
  const char *get_category() override { return "Conversation/Diagnostic"; }
  /// @brief 区別させるための名前
  const char *get_name() override { return "Wifi"; }

protected:
  /// @brief 初期化を実行する
  bool setup() override {
    if (nullptr != this->_serial) {
      this->_serial->begin(this->_baudrate);
      this->enable();
      return true;
    }
    return false;
  }
  size_t message(OUTPUT_LOG_LEVEL a_level, bool a_newline, const char *a_message) override {
    int index = 0;
    bool result = this->is_connect();
    if (true == result) {
      if (OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL == a_level) {
      } else if (OUTPUT_LOG_LEVEL::LEVEL_DEBUG <= a_level) {
        if (0 != target_diag.port) {
          String textLevel = this->get_text_level(a_level);
          uint8_t data[255 + textLevel.length()] = {0};
          data[index++] = '[';
          for (int i = 0; i < textLevel.length(); i++) {
            data[index++] = textLevel[i];
          }
          data[index++] = ']';
          data[index++] = ' ';
          for (int i = 0; i < 250; i++) {
            if (0x00 != a_message[i]) {
              data[index++] = a_message[i];
            } else {
              data[index] = 0x00; // 文字列の終端
              break;
            }
          }
          if (index > 0) {
            this->a_udp.beginPacket(target_diag.ip, target_diag.port); // UDPパケットの開始
            this->a_udp.write(data, index);                            // データの書き込み
            this->a_udp.endPacket();                                   // UDPパケットの終了
          }
        }
      }
    }
    if (nullptr != this->_serial) {
      if (OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL == a_level) {
        return this->_serial->printf("%s", a_message);
      } else {
#if DEBUG_MERIDIAN_CORE
        return this->_serial->printf("[%9.3f][%s] %s%s", millis() / 1000.0f, this->get_text_level(a_level), a_message, a_newline ? "\n" : "");
#else
        return this->_serial->printf("[%s] %s%s", this->get_text_level(a_level), a_message, a_newline ? "\n" : "");
#endif
      }
    }
    return index;
  }

  /// @brief 受信処理の実処理
  bool received(uint8_t *data, int *data_length) override {
    bool result = false;
    static int a_len = MERIDIM_BYTE_SIZE;
    *data_length = 0;
    int size = this->a_udp.parsePacket();
    if (size > 0) {
      if (size >= a_len) {
        if (0 < this->a_udp.read(data, a_len)) {
          result = true;
          *data_length = size;
          // TODO: 実装する
          // meridian::core::execution::mrd_convert_Meridim90(a_meridim, a_meridim_array, a_len);
          if (OUTPUT_LOG_LEVEL::LEVEL_TRACE >= this->get_level_unit()) {
            this->trace(mrd_format("MrdConversationWifi::received() %s:%d", this->_local_ip.toString().c_str(), this->_open_port).c_str());
            for (int i = 0; i < MERIDIM_BYTE_SIZE; i++) {
              this->operational(mrd_format("%02X ", data[i]).c_str());
            }
            this->operational("\n");
          }
        }
      } else {
        this->a_udp.read(data, size); // バッファの放棄
      }
    }
    return result; // バッファにデータがない
  }
  /// @brief 送信処理の実処理
  bool send(uint8_t *data) override {
    bool result = this->is_connect();
    if (true == result) {
      if (0 != target.port) {
        this->a_udp.beginPacket(target.ip, target.port); // UDPパケットの開始
        this->a_udp.write(data, MERIDIM_BYTE_SIZE);      // データの書き込み
        this->a_udp.endPacket();                         // UDPパケットの終了
      }
      if (OUTPUT_LOG_LEVEL::LEVEL_TRACE >= this->get_level_unit()) {
        this->trace(mrd_format("MrdConversationWifi::send() %s:%d", target.ip.toString().c_str(), target.port).c_str());
        for (int i = 0; i < MERIDIM_BYTE_SIZE; i++) {
          this->operational(mrd_format("%02X ", data[i]).c_str());
        }
        this->operational("\n");
      }
    }
    return result;
  }

public:
  /// @brief 診断用のインターフェースを登録する
  void set_diagnostic(IMrdDiagnostic &ref) override {
    this->diag = &ref;
  }
  /// @brief 現在のWiFiの接続状態を取得する
  bool is_connect() {
    bool result = (WiFi.status() != WL_CONNECTED) ? false : true;
    return result;
  }

  /// @brief WiFiと接続する
  bool connect(const char *ssid, const char *password, int open_port = 40009) {
    bool result = true;
    this->_open_port = open_port;
    WiFi.disconnect(true, true); // 新しい接続のためにWiFi接続をリセット
    delay(100);
    if (false == this->_host_name.equals("")) {
      WiFi.setHostname(this->_host_name.c_str()); // ホスト名を設定
    }
    WiFi.begin(ssid, password); // Wifiに接続
    int delay_ms = 50;
    int timeout_ms = 10 * (1000);
    int logging_time_ms = (500);

    bool flag_output_once = false;
    while (WiFi.status() != WL_CONNECTED) { // https://www.arduino.cc/en/Reference/WiFiStatus 戻り値一覧
      timeout_ms -= delay_ms;
      if (OUTPUT_LOG_LEVEL::LEVEL_DEBUG <= this->get_level_unit()) {
        if (0 == timeout_ms % logging_time_ms) { // 0.5秒ごとに接続状況を出力
          this->operational(".", false);
          flag_output_once = true;
        }
      }
      delay(delay_ms);       // 接続が完了するまでループで待つ
      if (0 >= timeout_ms) { // 10秒でタイムアウト
        this->warn("WiFi init TIMEOUT.");
        result = false;
      }
    }
    if ((OUTPUT_LOG_LEVEL::LEVEL_DEBUG <= this->get_level_unit()) && flag_output_once) {
      this->operational("\n");
    }
    if (true == result) {
      this->_local_ip = WiFi.localIP();
      this->a_udp.setTimeout(this->_timeout_ms);
      uint8_t ret = this->a_udp.begin(this->_open_port);
      if (0 != ret) {
      } else {
        result = false;
      }
    }

    if (true == result) {
      this->info(mrd_format("WiFi connected: %s, IP: %s, Port: %d", ssid, this->_local_ip.toString().c_str(), this->_open_port).c_str());
    } else {
      this->error(mrd_format("Failed to connect WiFi: %s", ssid).c_str());
    }
    return result;
  }

  /// @brief IPアドレスを取得する
  const char *get_ip_address() {
    return WiFi.localIP().toString().c_str();
  }
  /// @brief 接続先のIPアドレスを取得する
  bool add_target(const char *ip, uint16_t port) {
    target.ip.fromString(ip);
    target.port = port;
    return true;
  }
  /// @brief 接続先のIPアドレスをクリアする
  bool clear_target() {
    target.port = 0;
    return true;
  }
  bool add_target_diag(const char *ip, uint16_t port = 40075) {
    target_diag.ip.fromString(ip);
    target_diag.port = port;
    return true;
  }

private:
  struct target_send {
    IPAddress ip;      ///! 送信先IPアドレス
    uint16_t port = 0; ///! 送信先ポート
  };

  int _open_port = 40009; ///! 受信ポート
  String _host_name = ""; ///! ホスト名

  WiFiUDP a_udp;       ///! UDPオブジェクト
  int _timeout_ms = 5; ///! タイムアウト時間
  IPAddress _local_ip; ///! ローカルIPアドレス

  target_send target;      ///! 接続先のIPアドレス
  target_send target_diag; ///! 接続先のIPアドレス

  HardwareSerial *_serial;     ///! シリアルポート
  uint32_t _baudrate = 115200; ///! ボーレート
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_AND_DIAGNOSTIC_WIFI_HPP__
