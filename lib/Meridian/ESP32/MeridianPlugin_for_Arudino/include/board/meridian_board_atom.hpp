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

namespace meridian {
namespace board {

class MeridianBoardAtom {
public:
  MeridianBoardAtom() = default;
  ~MeridianBoardAtom() = default;

  bool Setup() {
    return true; // Setup successful
  }
  bool Loop() {
    return true; // Loop successful
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_MERIDIAN_BOARD_LITE_ATOM_HPP__
