/**
 * @file meridian_board_twin_for_esp32_setting.hpp
 * @brief 設定: "Meridian Board Twin for ESP32"
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_SETTING_HPP__
#define __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_SETTING_HPP__

namespace meridian {
namespace board {

#define SETTING_DEFAULT_TIMER_SECTION_US 100 ///! タイマーの分解能(Default:100us)
#define SETTING_DEFAULT_TIMER_DELAY_RATE 0.9 ///! スリープモードの割合

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_TWIN_FOR_ESP32_SETTING_HPP__
