/**
 * @file meridian_board_atom_.hpp
 * @brief "Meridian Board on M5Atom"のための定義をまとめたヘッダファイル
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__

#include "board/pins/meridian_board_atom_pins.hpp"
#include "board/setting/meridian_board_atom_setting.hpp"
#include "mrd_util_for_arduino.hpp"

#include <Arduino.h>
#include <interface/i_board.hpp>
#include <interface/i_mrd_communication_conversation.hpp>
#include <interface/i_mrd_communication_diagnostic.hpp>
#include <interface/i_mrd_plugin.hpp>

/// @brief バージョン情報の定義
#define MERIDIAN_VERSION BUILD_BOARD_NAME " ver." BUILD_VERSION
/// @brief タイマーのセクション時間(μs)
#define CALC_MS_TO_COUNTER(x) ((x) * (1000.0 / SETTING_DEFAULT_TIMER_SECTION_US))
#define CALC_COUNTER_TO_MS(x) ((x) * (SETTING_DEFAULT_TIMER_SECTION_US / 1000.0))

namespace meridian {
namespace board {
////////////////////////////////////////////////////////////////////////////////////////////////////
// 一定間隔を保つためのタイマー関数
//  * 一定間隔で動作するタイマー関数を使用して間隔をキープする
//  * タイマーの分解能は、「SETTING_DEFAULT_TIMER_SECTION_US」(Default:100us)で指定可能
//  * 「SETTING_DEFAULT_TIMER_DELAY_RATE」は「delay()」関数を呼び出すスリープ時間を決める割合
//     残りはホットスリープでタイム時間を監視する
//  * Arduinoのmillis()は、オーバーフローするため自作しています。
////////////////////////////////////////////////////////////////////////////////////////////////////
volatile bool count_frame_reset = false; // フレーム管理時計をリセットする

// ハードウェアタイマーとカウンタ用変数の定義
hw_timer_t *timer = NULL;                                ///! ハードウェアタイマーの設定
uint8_t timer_no = 0;                                    ///! タイマー番号
volatile SemaphoreHandle_t timer_semaphore;              ///! ハードウェアタイマー用のセマフォ
volatile unsigned long count_timer = 0;                  ///! フレーム用タイマーのカウントアップ
volatile bool flag_reset_count_timer = false;            ///! フレーム用タイマーのカウントリセット
volatile bool flag_reduce_count_timer = false;           ///! フレーム用タイマーのカウントを減少させる
unsigned long timer_waypoint = CALC_MS_TO_COUNTER(10.0); ///! タイマーの目標値

int OVERFLOW_CLEAR_COUNT_MAX = 10; ///! オーバーフロー発生を確認してリセットするまでのカウント数

unsigned long timer_delay_limit = CALC_MS_TO_COUNTER(10.0 * SETTING_DEFAULT_TIMER_DELAY_RATE); ///! スリープする閾値

#if DEBUG_MERIDIAN_CORE
volatile unsigned long previous_count_timer = 0; ///! 前回のカウント値
#endif

/// @brief count_timerを保護しつつ1ずつインクリメント
void IRAM_ATTR mrd_frame_timer() {
  count_timer++;
  if (flag_reduce_count_timer) {
    count_timer = count_timer % timer_waypoint;
    flag_reduce_count_timer = false;
#if DEBUG_MERIDIAN_CORE
    previous_count_timer = count_timer;
#endif
  }
  if (flag_reset_count_timer) {
    count_timer = 0;
    flag_reset_count_timer = false;
  }

  xSemaphoreGiveFromISR(timer_semaphore, NULL); // セマフォを与える
}

void mrd_timer_clear() { flag_reset_count_timer = true; }

void mrd_timer_setup(int a_duration_ms) {
  timer_waypoint = (unsigned long)CALC_MS_TO_COUNTER(a_duration_ms);
  timer_delay_limit = (unsigned long)CALC_MS_TO_COUNTER(a_duration_ms * SETTING_DEFAULT_TIMER_DELAY_RATE);
  // タイマーの初期化
  mrd_timer_clear();
  // タイマーの設定
  timer_semaphore = xSemaphoreCreateBinary();                      // セマフォの作成
  timer = timerBegin(timer_no, getApbFrequency() / 1000000, true); // タイマーの設定(1つ目のタイマーを使用, 分周比80)
  timerAttachInterrupt(timer, &mrd_frame_timer, false);            // frame_timer関数をタイマーの割り込みに登録
  timerAlarmWrite(timer, SETTING_DEFAULT_TIMER_SECTION_US, true);  // トリガー時間の設定
  timerAlarmEnable(timer);                                         // タイマーを開始
}
void mrd_timer_delay() {
#if DEBUG_MERIDIAN_CORE
  unsigned long running_count_timer = count_timer - previous_count_timer;
  int sleep_counting_hot = 0;
  int sleep_counting_delay = 0;
#endif
  static int overflow_count = OVERFLOW_CLEAR_COUNT_MAX;
  unsigned long current_count_timer = 0;
  int delay_time = 0;
  bool loop_count = false;
  bool flag_loop = true;
  while (flag_loop) {
    if (xSemaphoreTake(timer_semaphore, SETTING_DEFAULT_TIMER_SECTION_US) == pdTRUE) {
      current_count_timer = count_timer; // ハードウェアタイマーの値を読む
      if (current_count_timer >= timer_waypoint) {
        flag_reduce_count_timer = true;
        flag_loop = false;
        break;
      }
    }
    if (true == flag_loop) {
      if (current_count_timer < timer_delay_limit) {
        // cold sleep
        delay_time =                                                         //
            (int)CALC_COUNTER_TO_MS(timer_delay_limit - current_count_timer) //
            + (0 != (timer_delay_limit % current_count_timer) ? 1 : 0);      //
        delay(delay_time);
#if DEBUG_MERIDIAN_CORE
        sleep_counting_delay += delay_time;
#endif
      } else {
        // hot sleep
#if DEBUG_MERIDIAN_CORE
        if (0 == sleep_counting_hot) {
          sleep_counting_hot = current_count_timer;
        }
#endif
      }
      loop_count = true;
    }
  }
  if (false == loop_count) {
    overflow_count--;
    if (overflow_count < 0) {
      mrd_timer_clear();
      overflow_count = OVERFLOW_CLEAR_COUNT_MAX;
    }
#if DEBUG_MERIDIAN_CORE
    Serial.printf("[E] cycle: [%7.3f][BOARDER:%d(R:%d)] %d(R:%3d H:%3d S:%3d)\n", millis() / 100.0f,
                  timer_waypoint, timer_delay_limit, current_count_timer,
                  running_count_timer,
                  (int)(current_count_timer - sleep_counting_hot),
                  (int)CALC_MS_TO_COUNTER(sleep_counting_delay));
  } else {
    Serial.printf("[i] cycle: [%7.3f][BOARDER:%d(R:%d)] %d(R:%3d H:%3d S:%3d)\n", millis() / 100.0f,
                  timer_waypoint, timer_delay_limit, current_count_timer,
                  running_count_timer,
                  (int)(current_count_timer - sleep_counting_hot),
                  (int)CALC_MS_TO_COUNTER(sleep_counting_delay));
#endif
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class MeridianBoardAtom : public IBoard {
public:
  class Parameter {
  public:
    bool is_network_ap_mode = true;
    bool is_network_sta_mode = false;
    bool is_factory_mode = false;
    int delay_time_ms = 10;
  };
  class Plugin {
  public:
    IMeridianConversation *con = nullptr; ///! 通信用のインターフェース
    IMeridianDiagnostic *diag = nullptr;  ///! 診断用のインターフェース
    IMeridianPlugin *pad = nullptr;
  };

public:
  Plugin plugin;
  Parameter parameter;
  CallbackProcess event_process = nullptr;

public:
  MeridianBoardAtom() {}
  ~MeridianBoardAtom() {}

  bool setup() override {
    bool result = false;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    result = true;
    if (nullptr == this->plugin.diag) {
      this->plugin.diag = new IMeridianDiagnostic();
    }

    this->plugin.con->set_diagnostic(*this->plugin.diag);
    this->plugin.diag->setup();
    this->plugin.diag->log(this->_level, "==========================================");
    this->plugin.diag->log(this->_level, "Hello M5Stack Atom!");
    this->plugin.diag->log(this->_level, "Version: " MERIDIAN_VERSION);
    this->plugin.diag->log(this->_level, "Formatted Build Time: %s", GetTimeString(BUILD_TIME).c_str());

    ////////////////////////////////////////////////////////////////////////////////////////////////
    this->_is_setup = result;
    mrd_timer_setup(this->parameter.delay_time_ms);

    return this->_is_setup;
  }

public:
  bool input(Meridim90 &a_meridim) override {
    return true;
  }
  bool processing(Meridim90 &a_meridim) override {
    bool result = true;
    if (nullptr != this->event_process) {
      result = this->event_process(a_meridim);
    }
    return result;
  }
  bool output(Meridim90 a_meridim) override {
    return true;
  }
  void waiting() override {
    mrd_timer_delay();
  }

private:
  bool _is_setup = false;
  OUTPUT_LOG_LEVEL _level = OUTPUT_LOG_LEVEL::LEVEL_INFO;
};

} // namespace board
} // namespace meridian

#undef MERIDIAN_VERSION
#undef CALC_MS_TO_COUNTER
#undef CALC_COUNTER_TO_MS

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
