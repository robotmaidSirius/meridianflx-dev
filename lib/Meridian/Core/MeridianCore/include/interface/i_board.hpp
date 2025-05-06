/**
 * @file i_board.hpp
 * @brief ボードのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-27
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_BOARD_I_BOARD_HPP__
#define __MERIDIAN_BOARD_I_BOARD_HPP__

#include "Meridim90.hpp"

namespace meridian {
namespace board {

class IBoard {
public:
  typedef bool (*CallbackProcess)(Meridim90 &a_meridim);

public:
  IBoard() = default;
  virtual ~IBoard() = default;

  virtual bool setup() { return true; }
  virtual bool input(Meridim90 &a_meridim) { return true; }
  virtual bool processing(Meridim90 &a_meridim) { return true; }
  virtual bool output(Meridim90 a_meridim) { return true; }
  virtual void waiting() {}

  virtual bool loop(Meridim90 &a_meridim) {
    bool result = false;
    if (true == this->input(a_meridim)) {
      if (true == this->processing(a_meridim)) {
        result = this->output(a_meridim);
      }
    }
    this->waiting();
    return result;
  }
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_I_BOARD_HPP__
