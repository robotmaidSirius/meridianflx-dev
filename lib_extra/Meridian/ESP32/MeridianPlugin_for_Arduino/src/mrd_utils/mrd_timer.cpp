/**
 * @file mrd_timer.cpp
 * @brief
 * @version 1.0.0
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "mrd_utils/mrd_timer.hpp"

#include <Arduino.h>

namespace meridian {

volatile bool count_frame_reset = false; // フレーム管理時計をリセットする

// ハードウェアタイマーとカウンタ用変数の定義
hw_timer_t *timer = NULL;                      ///! ハードウェアタイマーの設定
uint8_t timer_no = 3;                          ///! タイマー番号
volatile SemaphoreHandle_t timer_semaphore;    ///! ハードウェアタイマー用のセマフォ
volatile unsigned long count_timer = 0;        ///! フレーム用タイマーのカウントアップ
volatile bool flag_reset_count_timer = false;  ///! フレーム用タイマーのカウントリセット
volatile bool flag_reduce_count_timer = false; ///! フレーム用タイマーのカウントを減少させる
int OVERFLOW_CLEAR_COUNT_MAX = 10;             ///! オーバーフロー発生を確認してリセットするまでのカウント数
unsigned long timer_waypoint = 100;            ///! タイマーの目標値
unsigned long timer_delay_limit = 90;          ///! スリープする閾値
uint64_t timer_section_us = 100;               ///! タイマーの分解能(μs)
float delay_rate = 0.9;                        ///! スリープモードの割合

#if DEBUG_MERIDIAN_CORE
volatile unsigned long previous_count_timer = 0; ///! 前回のカウント値
#endif

/// @brief タイマーのセクション時間(μs)
inline unsigned long CALC_US_TO_COUNTER(float time_us) { return (unsigned long)(time_us / timer_section_us); }
inline int CALC_COUNTER_TO_MS(int count) { return (int)((count) * (timer_section_us / 1000.0)); }

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
void mrd_timer_setup(int a_duration_us, uint64_t a_timer_section_us, float a_delay_rate) {
  timer_section_us = a_timer_section_us;
  delay_rate = a_delay_rate;
  timer_waypoint = (unsigned long)CALC_US_TO_COUNTER(a_duration_us);
  timer_delay_limit = (unsigned long)CALC_US_TO_COUNTER(a_duration_us * a_delay_rate);
  // タイマーの初期化
  mrd_timer_clear();
  // タイマーの設定
  timer_semaphore = xSemaphoreCreateBinary();                      // セマフォの作成
  timer = timerBegin(timer_no, getApbFrequency() / 1000000, true); // タイマーの設定(1つ目のタイマーを使用, 分周比80)
  timerAttachInterrupt(timer, &mrd_frame_timer, false);            // frame_timer関数をタイマーの割り込みに登録
  timerAlarmWrite(timer, timer_section_us, true);                  // トリガー時間の設定
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
    if (xSemaphoreTake(timer_semaphore, timer_section_us) == pdTRUE) {
      // ハードウェアタイマーの値を読む
      current_count_timer = count_timer;
      if (current_count_timer >= timer_waypoint) {
        flag_reduce_count_timer = true;
        flag_loop = false;
        break;
      }
    }
    if (true == flag_loop) {
      if (current_count_timer < timer_delay_limit) {
        // cold sleep
        delay_time =
            (int)CALC_COUNTER_TO_MS(timer_delay_limit - current_count_timer) +
            (0 != (timer_delay_limit % current_count_timer) ? 1 : 0);
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
    Serial.printf("[E] cycle: [%9.3f][BOARDER:%d(Sleep:%d)] %d(H:%3d S:%3d R:%3d)\n", millis() / 1000.0f,
                  timer_waypoint, timer_delay_limit,
                  current_count_timer,
                  (int)(current_count_timer - sleep_counting_hot), // Hot sleep
                  (int)CALC_US_TO_COUNTER(sleep_counting_delay),   // Cold sleep
                  running_count_timer);                            // Running count
  } else {
    Serial.printf("[i] cycle: [%9.3f][BOARDER:%d(Sleep:%d)] %d(H:%3d S:%3d R:%3d)\n", millis() / 1000.0f,
                  timer_waypoint, timer_delay_limit,
                  current_count_timer,
                  (int)(current_count_timer - sleep_counting_hot), // Hot sleep
                  (int)CALC_US_TO_COUNTER(sleep_counting_delay),   // Cold sleep
                  running_count_timer);                            // Running count
#endif
  }
}

} // namespace meridian
