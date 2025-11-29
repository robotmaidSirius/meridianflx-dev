/**
 * @file i_mrd_conversation.hpp
 * @brief 通信のためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_COMMUNICATION_I_MRD_CONVERSATION_HPP__
#define __MERIDIAN_COMMUNICATION_I_MRD_CONVERSATION_HPP__
// ヘッダファイルの読み込み
#include "Meridim.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"
// ライブラリ導入
#include <string.h>

namespace meridian {
namespace communication {

/// @brief 通信のためのインターフェイスクラス
/// @note Meridianの通信はこのクラスを継承して実装すること。
class IMrdConversation : public MeridianDiagnosticUnit {
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 仮想関数 - カテゴリー名
  virtual const char *get_category() { return "Conversation"; }
  /// @brief 仮想関数 - 区別させるための名前
  virtual const char *get_name() { return "NONE"; }

protected:
  /// @brief 仮想関数 - 通信の初期化
  virtual bool setup() { return true; }
  /// @brief 仮想関数 - 受信処理の実処理
  /// @param a_meridim 受信するデータ
  virtual bool received(uint8_t *data, int *data_length) {
    static uint8_t *buffer = new uint8_t[180]{0x00};
    *data_length = 180;
    data = buffer;
    return true;
  }
  /// @brief 仮想関数 - 送信処理の実処理
  /// @param a_meridim 送信するデータ
  virtual bool send(uint8_t *data) { return true; }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief コンストラクタ
  IMrdConversation() {
  }
  /// @brief 送信、受信をするか制御します。フラグがtrueの場合、送信、受信を行います。
  void config(bool call_func_received, bool call_func_send) {
    this->_is_received = call_func_received;
    this->_is_send = call_func_send;
  }
  /// @brief 初期化を実行する
  bool begin() {
    if (false == this->_is_initialized) {
      this->_is_initialized = this->setup();
    }
    return this->_is_initialized;
  }
  /// @brief 受信
  /// @param a_meridim 受信するデータ
  bool reception(Meridim &a_meridim) {
    bool result = false;
    if (true == this->_is_initialized) {
      if (true == this->_is_received) {
        uint8_t *data = new uint8_t[MERIDIM_BYTE_SIZE]{0x00};
        int data_length = 0;
        if (true == this->received(data, &data_length)) {
          // チェックサム確認
          int sum = 0;
          int checksum = 0;
          if (data_length > 2) {
            for (size_t i = 0; (i < data_length - 2) && (i < MERIDIM_DATA_LEN); i = i + 2) {
              sum = (sum + (data[i] & 0xFF) + ((data[i + 1] << 8) & 0xFF00)) & 0xFFFF;
            }
            checksum = (~((data[data_length - 2] & 0xFF) + ((data[data_length - 1] << 8) & 0xFF00))) & 0xFFFF;
          }
          if (checksum == sum) {
            result = true;
            set_meridim(a_meridim, data, data_length); // 受信したデータをMeridim型に変換
          } else {
            this->warn("Checksum is invalid");
          }
        } else {
          this->warn("Failed to receive data from conversation.");
        }
        delete[] data;
      }
    }
    return result;
  }
  /// @brief 送信
  /// @param a_meridim 送信するデータ
  bool transmission(Meridim &a_meridim) {
    if (true == this->_is_initialized) {
      if (true == this->_is_send) {
        mrd_countup(a_meridim); // カウントアップを行う
        uint8_t data[MERIDIM_BYTE_SIZE] = {0};
        memcpy(data, &a_meridim, MERIDIM_BYTE_SIZE);
        int sum = 0;
        for (size_t i = 0; i < MERIDIM_DATA_LEN - 1; ++i) {
          sum = (sum + ((data[i * 2] & 0xFF) + ((data[i * 2 + 1] << 8) & 0xFF00))) & 0xFFFF;
        }
        a_meridim.checksum = (~sum) & 0xFFFF;                           // チェックサムを設定 (1の補数)
        data[MERIDIM_BYTE_SIZE - 2] = (a_meridim.checksum) & 0xFF;      // 最後のバイトにチェックサムを設定
        data[MERIDIM_BYTE_SIZE - 1] = (a_meridim.checksum >> 8) & 0xFF; // 最後のバイトにチェックサムを設定
        return this->send(data);
      }
    }
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  bool _is_send = true;         ///! 送信許可フラグ
  bool _is_received = true;     ///! 受信許可フラグ
  bool _is_initialized = false; ///! 初期化フラグ
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_COMMUNICATION_I_MRD_CONVERSATION_HPP__
