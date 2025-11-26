/**
 * @file i_mrd_diagnostic.hpp
 * @brief ログ通知用のインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_DIAGNOSTIC_HPP__
#define __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_DIAGNOSTIC_HPP__

#include "Meridim.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MERIDIAN_COMMUNICATION_BUFFER_SIZE
/// @brief Diagnosticのバッファサイズ
/// @note デフォルトは256バイト
/// @note 256バイト以上のサイズが必要な場合は、MERIDIAN_COMMUNICATION_BUFFER_SIZEを定義してください。
/// @note 例: -D MERIDIAN_COMMUNICATION_BUFFER_SIZE=512
#define MERIDIAN_COMMUNICATION_BUFFER_SIZE 256
#endif

namespace meridian {
namespace communication {

/// @brief Define log level
typedef enum output_log_level_t {
  LEVEL_ALL,         /*!< Any logging levels that have been configured are logged at this log level. */
  LEVEL_TRACE,       /*!< The TRACE log level records all of the application's behaviour details. Its purpose is primarily diagnostic, and it is more granular and finer than the DEBUG log level. */
  LEVEL_DEBUG,       /*!< You are providing diagnostic information in a thorough manner with DEBUG. It's long and contains more information than you'll need when using the application. */
  LEVEL_INFO,        /*!< INFO messages are similar to how applications normally behave. */
  LEVEL_WARN,        /*!< When an unexpected application issue has been identified, the WARN log level is used.  This indicates that you are unsure if the issue will recur or not. At this time, you may not notice any negative effects on your application. */
  LEVEL_ERROR,       /*!< This log level is used when a serious issue is preventing the application's functionalities from functioning properly. */
  LEVEL_FATAL,       /*!< The FATAL level of logging indicates that the application's situation is critical, such as when a critical function fails. */
  LEVEL_OPERATIONAL, /*!< When This log level signals operational messages. */
  LEVEL_OFF          /*!< Nothing is logged at this level of logging. */
} OUTPUT_LOG_LEVEL;

/// @brief ログ通知用のインターフェイスクラス
/// @note このクラスを継承して実装すること。
class IMeridianDiagnostic {
  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 仮想関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief カテゴリー名
  virtual const char *get_category() { return "Diagnostic"; }
  /// @brief 仮想関数 - 名前を取得
  virtual const char *get_name() { return "None"; }

protected:
  /// @brief 仮想関数 - 初期化
  virtual bool setup() { return true; }
  /// @brief 仮想関数 - メッセージを出力
  /// @param level 出力レベル
  /// @param message 出力メッセージ
  virtual size_t message(OUTPUT_LOG_LEVEL level, bool a_newline, const char *message) { return 0; }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// 公開関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
public:
  /// @brief 初期化を実行する
  bool begin() { return this->setup(); }
  /// @brief 出力レベルの設定
  /// @param level 出力レベル
  void set_level(OUTPUT_LOG_LEVEL level) { this->_level = level; }
  /// @brief ログの出力を有効化
  void enable() { this->_output_log = true; }
  /// @brief ログの出力を無効化
  void disable() { this->_output_log = false; }

  /// @brief ログを出力
  /// @param a_level ログレベル
  /// @param format 出力フォーマット
  void log(OUTPUT_LOG_LEVEL a_level, bool a_newline, const char *format, ...) {
    if (this->_output_log) {
      if (this->_level <= a_level) {
        char loc_buf[this->_BUFFER_SIZE];
        char *message = loc_buf;
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        int len = vsnprintf(message, sizeof(loc_buf), format, copy);
        va_end(copy);
        if (len < 0) {
          va_end(arg);
          return;
        }
        if ((unsigned long long)len >= sizeof(loc_buf)) {
          message = (char *)malloc(len + 1);
          if (message == NULL) {
            va_end(arg);
            return;
          }
          len = vsnprintf(message, len + 1, format, arg);
        }
        va_end(arg);

        this->message(a_level, a_newline, message);
        if (message != loc_buf) {
          free(message);
        }
        return;
      }
    }
    return;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  /// protected 関数
  //////////////////////////////////////////////////////////////////////////////////////////////////
protected:
  /// @brief ログレベルからテキストを取得
  const char *get_text_level(OUTPUT_LOG_LEVEL level) {
    switch (level) {
    case OUTPUT_LOG_LEVEL::LEVEL_ALL:
      return "---";
    case OUTPUT_LOG_LEVEL::LEVEL_TRACE:
      return "TRA";
    case OUTPUT_LOG_LEVEL::LEVEL_DEBUG:
      return "DEB";
    case OUTPUT_LOG_LEVEL::LEVEL_INFO:
      return "INF";
    case OUTPUT_LOG_LEVEL::LEVEL_WARN:
      return "WAR";
    case OUTPUT_LOG_LEVEL::LEVEL_ERROR:
      return "ERR";
    case OUTPUT_LOG_LEVEL::LEVEL_FATAL:
      return "FAT";
    case OUTPUT_LOG_LEVEL::LEVEL_OFF:
      return "OFF";
    case OUTPUT_LOG_LEVEL::LEVEL_OPERATIONAL:
      return "OPT";
    default:
      return "UNK";
    }
  }

  bool _output_log = false;                                    ///! Output control flag
  const int _BUFFER_SIZE = MERIDIAN_COMMUNICATION_BUFFER_SIZE; ///! Buffer size
#ifdef MERIDIAN_DEFAULT_OVERALL_LOG_LEVEL
  OUTPUT_LOG_LEVEL _level = MERIDIAN_DEFAULT_OVERALL_LOG_LEVEL; ///! Output Level
#else
  OUTPUT_LOG_LEVEL _level = OUTPUT_LOG_LEVEL::LEVEL_WARN; ///! Output Level
#endif
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_DIAGNOSTIC_HPP__
