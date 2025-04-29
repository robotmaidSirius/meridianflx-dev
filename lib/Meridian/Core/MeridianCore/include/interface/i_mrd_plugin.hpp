/**
 * @file i_mrd_plugin.hpp
 * @brief プラグインのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
#define __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__

#include "Meridim90.hpp"
#include "interface/i_mrd_communication_diagnostic.hpp"

namespace meridian {
namespace modules {
namespace plugin {

using namespace meridian::core::communication;

class IMeridianPlugin {
public:
  class Status {
  public:
    Status(int a_option_max = 0) {
      this->OPTION_SIZE = a_option_max;

      // TODO: 配列を動的に確保する
      this->option = new bool[this->OPTION_SIZE];
      for (int i = 0; i < this->OPTION_SIZE; i++) {
        this->option[i] = false;
      }
    }

    int OPTION_SIZE = 0;
    bool initalized = false;
    bool setup = false;
    bool *option;
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
    for (int i = 0; i < this->state.OPTION_SIZE; i++) {
      a_state.option[i] = this->state.option[i];
    }
  }

protected:
  IMeridianDiagnostic *diag;
  Status state;

private:
#ifdef MERIDIAN_DEFAULT_LEVEL_LEVEL
  OUTPUT_LOG_LEVEL _level = MERIDIAN_DEFAULT_LEVEL_LEVEL;
#else
  OUTPUT_LOG_LEVEL _level = OUTPUT_LOG_LEVEL::LEVEL_WARN;
#endif
};

} // namespace plugin
} // namespace modules
} // namespace meridian

using namespace meridian::modules::plugin;

#endif // __MERIDIAN_MODULES_PLUGIN_I_MRD_PLUGIN_HPP__
