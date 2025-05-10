/**
 * @file i_mrd_communication_conversation.hpp
 * @brief 通信のためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__
#define __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__

#include "Meridim90.hpp"
#include "interface/i_mrd_communication_diagnostic_append.hpp"

namespace meridian {
namespace core {
namespace communication {

/// @brief 通信のためのインターフェイスクラス
/// @note Meridianの通信はこのクラスを継承して実装すること。
class IMeridianConversation : public IMeridianDiagnosticAppend {

protected:
  /// @brief 受信処理の実処理
  /// @param a_meridim 受信するデータ
  virtual bool receivedImpl(Meridim90 &a_meridim) { return true; }
  /// @brief 送信処理の実処理
  /// @param a_meridim 送信するデータ
  virtual bool sendImpl(Meridim90 &a_meridim) { return true; }

public:
  /// @brief 区別させるための名前
  virtual const char *get_name() { return "NONE"; }
  /// @brief 通信の初期化
  virtual bool setup() { return true; }

public:
  /// @brief 送信、受信をするか制御します。フラグがtrueの場合、送信、受信を行います。
  void config(bool call_func_received, bool call_func_send) {
    this->_is_received = call_func_received;
    this->_is_send = call_func_send;
  }

  /// @brief 受信
  /// @param a_meridim 受信するデータ
  bool received(Meridim90 &a_meridim) {
    if (true == this->_is_received) {
      return this->receivedImpl(a_meridim);
    }
    return true;
  }
  /// @brief 送信
  /// @param a_meridim 送信するデータ
  bool send(Meridim90 &a_meridim) {
    if (true == this->_is_send) {
      return this->sendImpl(a_meridim);
    }
    return true;
  }

private:
  bool _is_send = true;     ///! 送信許可フラグ
  bool _is_received = true; ///! 受信許可フラグ

#if false
public:
  class Status {
  public:
    bool initalized = false;
    bool setup = false;
    bool happened_error = false;
    bool no_device = false;

  public:
    void all_ok() {
      this->initalized = true;
      this->setup = true;
      this->happened_error = false;
      this->no_device = true;
    }
  };

public:
  void get_status(Status &state) {
    state.initalized = this->state.initalized;
    state.setup = this->state.setup;
    state.happened_error = this->state.happened_error;
    state.no_device = this->state.no_device;
  }

protected:
  Status state;
#endif
};

} // namespace communication
} // namespace core
} // namespace meridian

#endif // __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__
