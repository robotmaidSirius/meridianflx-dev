/**
 * @file i_mrd_board.hpp
 * @brief ボードのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_I_MRD_BOARD_HPP__
#define __MERIDIAN_BOARD_I_MRD_BOARD_HPP__

#include "Meridim.hpp"
#include "interface/i_mrd_app.hpp"
#include "interface/i_mrd_conversation.hpp"
#include "interface/i_mrd_driver.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

#include <list>

namespace meridian {
namespace board {

using namespace meridian::app;
using namespace meridian::modules;
using namespace meridian::communication;

/// @brief ボードのインターフェイスクラス
/// @note Meridianのボードはこのクラスを継承して実装すること。
class IMrdBoard : public meridian::communication::MeridianDiagnosticUnit {
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  ///   実装時には、以下の関数をオーバーライドすること
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  /// @brief ループ時の待機処理。ボードによってTimerの実装が異なるため、継承先で実装すること。
  /// @return 待機時間（マイクロ秒単位）
  virtual int waiting() = 0;
  /// @brief 初期化処理
  virtual bool setup() { return true; }
  /// @brief 入力処理
  virtual bool loop_input(Meridim &a_meridim) { return true; }
  /// @brief 処理の前処理
  virtual bool loop_process_prepare(Meridim &a_meridim) { return true; }
  /// @brief 処理の片付け
  virtual bool loop_process_tidy_up(Meridim &a_meridim) { return true; }
  /// @brief 出力処理
  virtual bool loop_output(Meridim &a_meridim) { return true; }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// public 変数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  Meridim meridim;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 初期化を実行する
  bool begin() {
    bool result = false;
    if (false == this->_is_initialized) {
      if (nullptr == this->diag) {
        this->diag = new IMeridianDiagnostic();
      }
      if (nullptr == this->con) {
        this->con = new IMeridianConversation();
      }
      result = this->diag->begin();
      if (true == result) {
        if (nullptr != this->con) {
          if (false == this->con->begin()) {
            this->error("Failed to initialize conversation interface.");
          }
        }
      } else {
        // 通知するためのdiagnosticが初期化されていないため、出力できない
        // this->error("Failed to initialize diagnostic unit.");
      }
      if (true == result) {
        for (IMeridianDriver *module : this->modules) {
          if (nullptr != module) {
            if (true == module->is_output) {
              result &= module->begin();
              if (false == result) {
                this->error("Failed to initialize module: %s", module->get_name());
              }
            }
          }
        }
      }
      if (true == result) {
        for (auto &ap : this->app) {
          if (nullptr != ap) {
            if (true == ap->is_enabled()) {
              result &= ap->begin(this->meridim);
              if (false == result) {
                this->error("Failed to initialize app: %s", ap->get_name());
              }
            }
          }
        }
      }
      if (true == result) {
        result = this->setup();
      }
    }
    this->_is_initialized = result;
    return result;
  }
  /// @brief 処理を実行する
  /// @param a_meridim Meridimの構造体
  /// @return true:正常終了, false:異常終了
  /// @note 実行順番は以下の通り
  /// * Input
  ///   01. [Conversation]受信処理
  ///     02. [継承先クラスの実装]input関数の処理
  ///       03. [Modules]モジュールの入力処理
  /// * Process
  ///         04. [継承先クラスの実装]Processの前処理
  ///           05. [App]アプリの実行
  ///         06. [継承先クラスの実装]Processの片付け
  /// * output
  ///       07. [Modules]モジュールの出力処理
  ///     08. [継承先クラスの実装]output関数の出力処理
  ///   09. [Conversation]送信処理
  bool update() {
    // TODO:作り変えます。
    //     Loop周期は10msでなくてもよいが、モジュール呼び出しタイミングは任意にする
    //     簡単なのはTimesだが、msで管理する
    //     前回から何秒立っているか確認する必要がある。
    bool result = this->_is_initialized;
    static int elapsed_time_us = 0;

    // 受信処理
    if (true == result) {
      if (nullptr != this->con) {
        if (true == this->con->reception(this->meridim)) {
          if (nullptr != this->diag) {
            if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
              this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_DEBUG, true, "<run> Received Data[seq:%04X]", this->meridim.sequential);
            }
          }
        }
      }
    }

    // 継承されたinput関数の処理
    if (true == result) {
      result = this->loop_input(this->meridim);
      if (nullptr != this->diag) {
        if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
          this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Board Input");
        }
      }
    }

    // 登録されたモジュールの入力処理
    if (true == result) {
      for (IMeridianDriver *module : this->modules) {
        if (nullptr != module) {
          if (true == module->is_input) {
            result &= module->reception(this->meridim);
            if (nullptr != this->diag) {
              if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
                this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Module [%s] Reception", module->get_name());
              }
            }
          }
        }
      }
    }

    // 継承されたProcessの前処理
    if (true == result) {
      result = this->loop_process_prepare(this->meridim);
      if (nullptr != this->diag) {
        if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
          this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Process Prepare");
        }
      }
    }

    // 登録されたモジュールの処理
    if (true == result) {
      for (auto &ap : this->app) {
        if (nullptr != ap) {
          if (true == ap->is_enabled()) {
            result &= ap->update(this->meridim, elapsed_time_us);
            if (nullptr != this->diag) {
              if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
                this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> App [%s] Process", ap->get_name());
              }
            }
          }
        }
      }
    }

    // 継承されたProcessの片付け
    if (true == result) {
      result = this->loop_process_tidy_up(this->meridim);
      if (nullptr != this->diag) {
        if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
          this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Process Tidy Up");
        }
      }
    }

    // モジュールの出力処理
    if (true == result) {
      for (IMeridianDriver *module : this->modules) {
        if (nullptr != module) {
          if (true == module->is_output) {
            result &= module->transmission(this->meridim);
            if (nullptr != this->diag) {
              if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
                this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Module [%s] Transmission", module->get_name());
              }
            }
          }
        }
      }
    }

    // 継承されたoutput関数の出力処理
    if (true == result) {
      result = this->loop_output(this->meridim);
      if (nullptr != this->diag) {
        if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
          this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Board Output");
        }
      }
    }

    // 送信処理
    if (true == result) {
      if (nullptr != this->con) {
        result = this->con->transmission(this->meridim);
        if (nullptr != this->diag) {
          if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
            this->diag->log(result ? OUTPUT_LOG_LEVEL::LEVEL_DEBUG : OUTPUT_LOG_LEVEL::LEVEL_WARN, true, "<run> Send Data[seq:%04X]", this->meridim.sequential);
          }
        }
      }
    }
    // 待機処理
    elapsed_time_us = this->waiting();
    return result;
  }
  /// @brief communicationを登録する
  void push_communication(IMeridianConversation *a_con, IMeridianDiagnostic *a_diag) {
    this->diag = a_diag;
    this->con = a_con;
    this->trace("push_communication() called");
    // 初期化されていない場合は、デフォルトのインスタンスを作成
    if (nullptr == this->diag) {
      this->diag = new IMeridianDiagnostic();
    }
    if (nullptr == this->con) {
      this->con = new IMeridianConversation();
    }
    // diagの設定を追加
    if (nullptr != this->diag) {
      if (nullptr != this->con) {
        this->con->set_diagnostic(*this->diag);
      }
      for (IMeridianDriver *module : this->modules) {
        if (nullptr != module) {
          module->set_diagnostic(*this->diag);
        }
      }
      for (auto &ap : this->app) {
        if (nullptr != ap) {
          ap->set_diagnostic(*this->diag);
        }
      }
    }
  }
  /// @brief communicationを登録する
  void push_communication(IMeridianDiagnostic *a_diag) {
    this->diag = a_diag;
    this->trace("push_communication() called");
    // 初期化されていない場合は、デフォルトのインスタンスを作成
    if (nullptr == this->diag) {
      this->diag = new IMeridianDiagnostic();
    }
    if (nullptr != this->diag) {
      if (nullptr != this->con) {
        this->con->set_diagnostic(*this->diag);
      }
      for (IMeridianDriver *module : this->modules) {
        if (nullptr != module) {
          module->set_diagnostic(*this->diag);
        }
      }
      for (auto &ap : this->app) {
        if (nullptr != ap) {
          ap->set_diagnostic(*this->diag);
        }
      }
    }
  }
  void push_communication(IMeridianConversation *a_con) {
    this->con = a_con;
    this->trace("push_communication() called");
    if (nullptr == this->con) {
      this->con = new IMeridianConversation();
    }
    if (nullptr != this->diag) {
      if (nullptr != this->con) {
        this->con->set_diagnostic(*this->diag);
      }
    }
  }
  /// @brief モジュールを登録します。
  bool push_module(IMeridianDriver *a_module) {
    if (nullptr != a_module) {
      if (nullptr != this->diag) {
        a_module->set_diagnostic(*this->diag);
        if (nullptr != this->diag) {
          if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_TRACE) {
            this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_TRACE, true, "<push_module> Module [%s] added", a_module->get_name());
          }
        }
      }
      this->modules.push_back(a_module);
      return true;
    }
    return false;
  }
  /// @brief アプリを登録します。
  bool push_app(IMeridianApp *a_app) {
    if (nullptr != a_app) {
      if (nullptr != this->diag) {
        a_app->set_diagnostic(*this->diag);
        if (nullptr != this->diag) {
          if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_TRACE) {
            this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_TRACE, true, "<push_app> App [%s] added", a_app->get_name());
          }
        }
      }
      this->app.push_back(a_app);
      return true;
    }
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 変数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  IMeridianConversation *con = nullptr; ///! 通信用のインスタンス
  std::list<IMeridianDriver *> modules; ///! modulesのインスタンス
  std::list<IMeridianApp *> app;        ///! プラグインのインターフェース
  bool _is_initialized = false;         ///! 初期化フラグ
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_I_MRD_BOARD_HPP__
