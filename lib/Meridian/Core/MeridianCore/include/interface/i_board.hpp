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
  typedef void (*CallbackProcess)(Meridim90 &a_meridim);

public:
  IBoard() = default;
  virtual ~IBoard() = default;

  virtual bool Setup() = 0;
  virtual bool Loop(Meridim90 &a_meridim) {
    bool result = false;
    // Meridim90 a_meridim;

    if (true == this->Input(a_meridim)) {
      this->Processing(a_meridim);
      result = this->Output(a_meridim);
    }
    return result;
  }

  virtual bool Input(Meridim90 &a_meridim) = 0;
  virtual bool Processing(Meridim90 &a_meridim) = 0;
  virtual bool Output(Meridim90 a_meridim) = 0;
};

} // namespace board
} // namespace meridian

#endif // __MERIDIAN_BOARD_I_BOARD_HPP__
