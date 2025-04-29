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
#include "interface/i_mrd_communication_diagnostic.hpp"

namespace meridian {
namespace core {
namespace communication {

class IMeridianConversation {
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
  virtual const char *get_name() { return "Conversation-None"; }
  virtual bool setup() { return true; }

private:
  virtual bool received(Meridim90 &a_meridim) { return false; }
  virtual bool send(Meridim90 &a_meridim) { return false; }

public:
  /// @brief 送信、受信をするか制御します。フラグがtrueの場合、送信、受信を行います。
  void notify_config(bool call_func_received, bool call_func_send) {
    this->_is_received = call_func_received;
    this->_is_send = call_func_send;
  }

  bool notify_received(Meridim90 &a_meridim) {
    if (true == this->_is_received) {
      return this->received(a_meridim);
    }
    return false;
  }
  bool notify_send(Meridim90 &a_meridim) {
    if (true == this->_is_received) {
      return this->send(a_meridim);
    }
    return false;
  }

  virtual void set_diagnostic(IMeridianDiagnostic &ref) { this->m_diag = &ref; }
  void get_status(Status &state) {
    state.initalized = this->m_state.initalized;
    state.setup = this->m_state.setup;
    state.happened_error = this->m_state.happened_error;
    state.no_device = this->m_state.no_device;
  }

protected:
  IMeridianDiagnostic *m_diag;
  Status m_state;

private:
  bool _is_send = true;
  bool _is_received = true;
};

} // namespace communication
} // namespace core
} // namespace meridian

#endif // __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__
