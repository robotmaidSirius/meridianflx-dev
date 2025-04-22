/**
 * @file i_mrd_plugin.hpp
 * @brief プラグインのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
#define __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__

#include "Meridim90.hpp"
#include "interface/i_mrd_communication_diagnostic.hpp"

namespace meridian {
namespace modules {
namespace plugin {

using namespace meridian::core::meridim;

template <int N = 1>
class IMeridianPlugin {
public:
  class Status {
  public:
    static const int OPTION_MAX = N;

    bool initalized = false;
    bool setup = false;
    bool option[Status::OPTION_MAX] = {false};
  };

public:
  virtual const char *get_name() { return "Unknow"; }
  virtual bool setup() = 0;
  virtual bool input(Meridim90 &a_meridim) = 0;
  virtual bool output(Meridim90 &a_meridim) = 0;

public:
  virtual void set_diagnostic(meridian::core::communication::IMeridianDiagnostic &a_ref) {
    this->diag = &a_ref;
  }

  void get_status(Status &a_state) {
    a_state.initalized = this->state.initalized;
    a_state.setup = this->state.setup;
    for (int i = 0; i < Status::OPTION_MAX; i++) {
      a_state.option[i] = this->state.option[i];
    }
  }

protected:
  meridian::core::communication::IMeridianDiagnostic *diag;
  Status state;
};

} // namespace plugin
} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
