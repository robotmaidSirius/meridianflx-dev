/**
 * @file board_lite_esp32.hpp
 * @brief Meridian LITE [Board: ESP32]のボード設定ファイル
 * @version 0.1.0
 * @date 2025-11-24
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __BOARD_LITE_ESP32_HPP__
#define __BOARD_LITE_ESP32_HPP__
#include "app_lite_esp32.hpp"
#include "meridian_network_keys.hpp"
#include "meridian_parameter.hpp"
#include <board/meridian_board_lite_for_esp32.hpp>

#include <mrd_communication/mrd_diagnostic_uart.hpp>
// #include <mrd_communication/mrd_conversation_wired_LAN_W5500.hpp>
#include <mrd_communication/mrd_conversation_wifi.hpp>
// #include <mrd_communication/mrd_con_and_diag_wifi.hpp>

namespace meridian {

class BoardSetting : public board::MeridianBoardLiteForEsp32 {
private:
  meridian::TestApp app;
  communication::MrdDiagnosticUart *diag = new communication::MrdDiagnosticUart(&Serial);
  // MrdConversationWiredLAN *com = new MrdConversationWiredLAN(5, 15, NETWORK_HOST_NAME);
  communication::MrdConversationWifi *com = new communication::MrdConversationWifi(nullptr, NETWORK_HOST_NAME);
  // MrdConversationAndDiagnosticWifi *com = new MrdConversationAndDiagnosticWifi(&Serial);

public:
  BoardSetting() {}
  ~BoardSetting() {}
  /// @brief 区別させるための名前
  const char *get_name() override { return MERIDIAN_BOARD_NAME; }

protected:
  /// @brief ボードの初期化処理
  bool init() override {
    //////////////////////////////////////////////////////////
    // ログレベル変更
    //////////////////////////////////////////////////////////
    this->com->set_log_level_unit(OUTPUT_LOG_LEVEL::LEVEL_ALL);
    this->app.set_log_level_unit(OUTPUT_LOG_LEVEL::LEVEL_ALL);
    this->set_log_level_unit(OUTPUT_LOG_LEVEL::LEVEL_ALL);
    //////////////////////////////////////////////////////////
    // ボード設定
    //////////////////////////////////////////////////////////
    this->push_communication(this->diag);
    this->push_communication(this->com);

    //////////////////////////////////////////////////////////
    // moduleの設定
    //////////////////////////////////////////////////////////
    // this->push_module(this->servo_pwm1);

    //////////////////////////////////////////////////////////
    // アプリケーションの設定
    //////////////////////////////////////////////////////////
    this->push_app(&this->app);
    return true;
  }
  /// @brief プラグインのセットアップ処理
  bool setup() override {
    // comの設定
    this->com->config(false, false); // 受信と送信を有効化
    this->com->add_target(NETWORK_SEND_IP, NETWORK_SEND_PORT);
#ifdef WIFI_LOG_IP
    this->com->add_target_diag(WIFI_LOG_IP);
#endif
    this->com->connect(NETWORK_WIFI_SSID, NETWORK_WIFI_PASS);

    return true;
  }
  /// @brief 入力処理
  bool loop_input(Meridim &a_meridim) override { return true; }
  /// @brief 処理の前処理
  bool loop_process_prepare(Meridim &a_meridim) override { return true; }
  /// @brief 処理の片付け
  bool loop_process_tidy_up(Meridim &a_meridim) override { return true; }
  /// @brief 出力処理
  bool loop_output(Meridim &a_meridim) override { return true; }
};

} // namespace meridian

#endif // __BOARD_LITE_ESP32_HPP__
