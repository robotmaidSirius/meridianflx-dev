/**
 * @file meridian_diagnostic_unit.hpp
 * @brief ログ通知用のインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_COMMUNICATION_MERIDIAN_DIAGNOSTIC_UNIT_HPP__
#define __MERIDIAN_COMMUNICATION_MERIDIAN_DIAGNOSTIC_UNIT_HPP__
// ヘッダファイルの読み込み
#include "interface/i_mrd_diagnostic.hpp"

namespace meridian {
namespace communication {

/// @brief モジュールなどの個別制御したい場合の継承クラス
class MeridianDiagnosticUnit {
public:
  /// @brief インスタンスの登録
  virtual void set_diagnostic(IMrdDiagnostic &a_ref) { this->diag = &a_ref; }

  // 全体のログレベルを設定
  /// @param a_level ログレベル
  void set_log_level_system(OUTPUT_LOG_LEVEL a_level) {
    if (nullptr != this->diag) {
      this->diag->set_level_system(a_level);
    }
  }
  /// @brief 個別のログレベルを設定
  /// @param a_level ログレベル
  void set_log_level_unit(OUTPUT_LOG_LEVEL a_level) {
    this->_unit_level = a_level;
  }
  OUTPUT_LOG_LEVEL get_level_unit() {
    return this->_unit_level;
  }
  OUTPUT_LOG_LEVEL get_level_system() {
    if (nullptr != this->diag) {
      return this->diag->get_level_system();
    } else {
      return this->_unit_level;
    }
  }

public:
  /// @brief Traceレベルのログを出力
  void trace(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_TRACE) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_TRACE, a_newline, text);
      }
    }
  }
  /// @brief Debugレベルのログを出力
  void debug(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_DEBUG) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_DEBUG, a_newline, text);
      }
    }
  }
  /// @brief Infoレベルのログを出力
  void info(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_INFO) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_INFO, a_newline, text);
      }
    }
  }
  /// @brief Warnレベルのログを出力
  void warn(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_WARN) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_WARN, a_newline, text);
      }
    }
  }
  /// @brief Errorレベルのログを出力
  void error(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_ERROR) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_ERROR, a_newline, text);
      }
    }
  }
  /// @brief Fatalレベルのログを出力
  void fatal(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_FATAL) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_FATAL, a_newline, text);
      }
    }
  }
  /// @brief Operationalレベルのログを出力
  void operational(const char *text, bool a_newline = true) {
    if (nullptr != this->diag) {
      if (this->_unit_level <= OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL) {
        this->diag->log(OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL, a_newline, text);
      }
    }
  }

protected:
  IMrdDiagnostic *diag = nullptr; ///! 診断用のインターフェース

private:
#ifdef MERIDIAN_DEFAULT_LOG_LEVEL
  OUTPUT_LOG_LEVEL _unit_level = MERIDIAN_DEFAULT_LOG_LEVEL; ///! 出力レベル
#else
  OUTPUT_LOG_LEVEL _unit_level = OUTPUT_LOG_LEVEL::LEVEL_WARN; ///! 出力レベル
#endif
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_COMMUNICATION_MERIDIAN_DIAGNOSTIC_UNIT_HPP__
