/**
 * @file mrd_conversation_wired_LAN_W5500.hpp
 * @brief
 * @version 0.1.0
 * @date 2025-11-29
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_WIRED_LAN_W5500_HPP__
#define __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_WIRED_LAN_W5500_HPP__
// ヘッダファイルの読み込み
#include "mrd_modules/gpio/mrd_module_gpio_out.hpp"
#include <interface/i_mrd_conversation.hpp>
#include <interface/i_mrd_diagnostic.hpp>
#include <meridian_core.hpp>
// ライブラリ導入
#include <Ethernet3.h>
#include <EthernetUdp3.h>
#include <SPI.h>

namespace meridian {
namespace communication {

class MrdConversationWiredLAN : public IMrdConversation {
public:
  /// @brief コンストラクタ
  MrdConversationWiredLAN(uint8_t cs_pin, uint8_t reset_pin = 0xFF, String hostName = "") : IMrdConversation() {
    this->_cs_pin = cs_pin;
    this->_reset_pin = reset_pin;
    this->_host_name = hostName;
  }

public:
  /// @brief カテゴリー名
  const char *get_category() override { return "Conversation"; }
  /// @brief 区別させるための名前
  const char *get_name() override { return "WiredLAN-W5500"; }

protected:
  /// @brief 初期化を実行する
  bool setup() override {
    bool result = false;
    if (0xFF != this->_cs_pin) {
      Ethernet.setCsPin(this->_cs_pin);
      if (0xFF != this->_reset_pin) {
        Ethernet.setRstPin(this->_reset_pin);
      }
      if (false == this->_host_name.equals("")) {
        Ethernet.setHostname(this->_host_name.c_str()); // ホスト名を設定
      }
      Ethernet.init();
      Ethernet.setRtTimeOut(this->_timeout_ms); // タイムアウト時間を設定
      result = true;
      this->_is_initialized = result;
    }
    return result;
  }

  /// @brief 受信処理の実処理
  bool received(uint8_t *data, int *data_length) override {
    static int a_len = MERIDIM_BYTE_SIZE;
    *data_length = 0;
    if (this->a_udp.available() >= a_len) {
      if (this->a_udp.parsePacket() >= a_len) { // データの受信バッファ確認

        int size = this->a_udp.read(data, a_len); // データの受信
        if (size > 0) {
          *data_length = size;
          // TODO: 実装する
          // meridian::core::execution::mrd_convert_Meridim90(a_meridim, a_meridim_array, a_len);
        }

        return true;
      }
    }
    return false; // バッファにデータがない
  }
  /// @brief 送信処理の実処理
  bool send(uint8_t *a_data) override {
    bool result = this->is_connect();
    if (true == result) {
#if false
      // TODO: 処理を記載する
      meridian::core::execution::meridim_countup(a_meridim);
      uint8_t a_meridim_array[MERIDIM_BYTE_SIZE] = {0};
      meridian::core::execution::mrd_convert_array(a_meridim_array, MERIDIM_BYTE_SIZE, a_meridim);
      this->a_udp.beginPacket(target.ip, target.port);        // UDPパケットの開始
      this->a_udp.write(a_meridim_array, MERIDIM_BYTE_SIZE); // データの書き込み
      this->a_udp.endPacket();                                // UDPパケットの終了
#endif
    }
    return result;
  }

  uint8_t *to_MAC(String mac) {
    static uint8_t mac_str[6];
    char temp[3];
    int i;
    for (i = 0; i < 6; i++) {
      if (mac.length() >= (i * 3 + 2)) {
        temp[0] = mac[i * 3 + 0];
        temp[1] = mac[i * 3 + 1];
        temp[2] = 0x00;
        mac_str[i] = strtol(temp, NULL, 16);
      }
    }
    return (uint8_t *)mac_str;
  }

public:
  /// @brief 現在のWiFiの接続状態を取得する
  bool is_connect() {
    if (1 == Ethernet.link()) {
      return true; // 接続されている
    } else {
      return false; // 接続されていない
    }
  }

  /// @brief WiFiと接続する
  bool connect(String mac_address = "", int open_port = 40009) {
    bool result = false;
    if (false == this->_is_initialized) {
      this->setup();
    }
    this->_open_port = open_port;
    uint8_t *mac = new uint8_t[6]{0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (mac_address.equals("") || mac_address.length() < 17) {
      for (int i = 0; i < 6; i++) {
        mac[i] = random(0, 255);
      }
      mac[0] = (mac[0] | 0x02) & 0xFE; // 1bit目：ユニキャスト ビット 2bit目：ローカル管理アドレス
    } else {
      mac = this->to_MAC(mac_address);
    }
    if (1 == Ethernet.begin(mac)) {
      // Succeeded
      result = true;
    }
    if (true == result) {
      result = this->a_udp.begin(open_port);
    }
    return result;
  }
  bool connect(String mac_address, IPAddress local_ip, IPAddress subnet, IPAddress gateway, int open_port = 40009) {
    bool result = false;
    this->_open_port = open_port;
    if (false == this->_is_initialized) {
      this->setup();
    }
    uint8_t *mac = this->to_MAC(mac_address);
    Ethernet.begin(mac, local_ip, subnet, gateway);
    result = this->a_udp.begin(open_port);
    return result;
  }
  bool connect(String mac_address, IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server, int open_port = 40009) {
    bool result = false;
    this->_open_port = open_port;
    if (false == this->_is_initialized) {
      this->setup();
    }
    uint8_t *mac = new uint8_t[6]{0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (mac_address.equals("") || mac_address.length() < 17) {
      for (int i = 1; i < 6; i++) {
        mac[i] = random(0, 255);
      }
    } else {
      mac = this->to_MAC(mac_address);
    }
    Ethernet.begin(mac, local_ip, subnet, gateway, dns_server);
    result = this->a_udp.begin(open_port);
    return result;
  }

  /// @brief IPアドレスを取得する
  const char *get_ip_address() {
    return Ethernet.localIP().toString().c_str();
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

private:
  int _open_port = 40009; ///! 受信ポート
  uint8_t _cs_pin = 0xFF;
  uint8_t _reset_pin = 0xFF;
  bool _is_initialized = false; ///! 初期化フラグ
  String _host_name = "";       ///! ホスト名

  EthernetUDP a_udp; ///! UDPオブジェクト
  struct target_send {
    IPAddress ip;      ///! 送信先IPアドレス
    uint16_t port = 0; ///! 送信先ポート
  };
  bool _output_log = true; ///! ログ出力フラグ
  int _timeout_ms = 5;     ///! タイムアウト時間
  target_send target;
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_COMMUNICATION_MRD_CONVERSATION_WIRED_LAN_W5500_HPP__
