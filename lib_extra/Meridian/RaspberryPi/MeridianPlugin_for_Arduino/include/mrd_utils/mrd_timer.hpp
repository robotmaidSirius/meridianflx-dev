/**
 * @file mrd_timer.hpp
 * @brief 一定間隔を保つためのタイマー関数
 * @version 1.0.0
 * @date 2025-05-06
 * @details 一定間隔を保つためのタイマー関数
 *   * 一定間隔で動作するタイマー関数を使用して間隔をキープする
 *   * タイマーの分解能は、「SETTING_DEFAULT_TIMER_SECTION_US」(Default:100us)で指定可能
 *   * 「SETTING_DEFAULT_TIMER_DELAY_RATE」は「delay()」関数を呼び出すスリープ時間を決める割合
 *      残りはホットスリープでタイム時間を監視する
 *   * Arduinoのmillis()は、オーバーフローするため自作しています。
 * @copyright Copyright (c) 2025
 */
#ifndef __MERIDIAN_UTILS_MRD_TIMER_HPP__
#define __MERIDIAN_UTILS_MRD_TIMER_HPP__
// ライブラリ導入
#include <Arduino.h>

namespace meridian {

/**
 * @brief タイマーの設定
 * @param a_duration_ms : タイマーの動作時間(ミリ秒)
 * @param timer_section_us : タイマーの分解能(マイクロ秒)
 * @param delay_rate : スリープ時間の割合
 */
void mrd_timer_setup(int a_duration_us, uint64_t timer_section_us, float delay_rate = 0.9);
/**
 * @brief タイマーの遅延
 * @details mrd_timer_setup()で指定した時間の間隔まで待機する
 */
void mrd_timer_delay();
/**
 * @brief タイマーのクリア
 */
void mrd_timer_clear();

} // namespace meridian

#endif // __MERIDIAN_UTILS_MRD_TIMER_HPP__
