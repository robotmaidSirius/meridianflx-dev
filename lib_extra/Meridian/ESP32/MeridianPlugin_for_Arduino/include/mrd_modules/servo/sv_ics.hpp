/**
 * @file sv_ics.h
 * @brief
 * @version 0.1.0
 * @date 2025-11-29
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULE_MRD_SERVO_ICS_HPP__
#define __MERIDIAN_MODULE_MRD_SERVO_ICS_HPP__
// ヘッダファイルの読み込み
#include "mrd_modules/common.hpp"
//  ライブラリ導入
#include <Arduino.h>
#include <IcsHardSerialClass.h>
#include <gs2d_krs.h>

#define SERVO_LOST_ERR_WAIT 6 // 連続何フレームサーボ信号をロストしたら異常とするか

//==================================================================================================
//  KONDO ICSサーボ関連の処理
//==================================================================================================

int mrd_Deg2Krs(float degree, float trim, int cw) {
  float _x = 7500 + (trim * 29.6296) + (degree * 29.6296 * cw);
  if (_x > 11500) // max limit
  {
    _x = 11500;
  } else if (_x < 3500) // min limit
  {
    _x = 3500;
  }
  return static_cast<int>(_x);
}
float mrd_Krs2Deg(int krs, float trim, int cw) {
  float _x = (krs - 7500 - (trim * 29.62963)) * 0.03375 * cw;
  return _x;
}

/// @brief ICSサーボの実行処理を行う関数
/// @param a_servo_id サーボのインデックス番号
/// @param a_cmd サーボのコマンド
/// @param a_tgt サーボの目標位置
/// @param a_tgt_past 前回のサーボの目標位置
/// @param a_tgt_trim サーボの補正値
/// @param a_cw サーボの回転方向補正値
/// @param a_err_cnt サーボのエラーカウント
/// @param a_stat サーボのステータス
/// @param ics サーボクラスのインスタンス
float mrd_servo_process_ics(int a_servo_id, int a_cmd, float a_tgt, float a_tgt_past, int a_trim,
                            int a_cw, int &a_err_cnt, uint16_t &a_stat, IcsHardSerialClass &ics) {
  int val_tmp = 0;
  if (a_cmd == 1) { // コマンドが1ならPos指定
    val_tmp = ics.setPos(a_servo_id, mrd_Deg2Krs(a_tgt, a_trim, a_cw));
  } else { // コマンドが0等なら脱力して値を取得
    val_tmp = ics.setFree(a_servo_id);
  }

  if (val_tmp == -1) { // サーボからの返信信号を受け取れなかった場合
    val_tmp = mrd_Deg2Krs(a_tgt_past, a_trim, a_cw);
    a_err_cnt++;
    if (a_err_cnt >= SERVO_LOST_ERR_WAIT) { // 一定以上の連続エラーで通信不能とみなす
      a_err_cnt = SERVO_LOST_ERR_WAIT;
      a_stat = 1;
    }
  } else {
    a_err_cnt = 0;
    a_stat = 0;
  }

  return mrd_Krs2Deg(val_tmp, a_trim, a_cw);
}

/// @brief ICSサーボを駆動する関数
/// @param a_meridim Meridimデータの参照
/// @param a_sv サーボパラメータの配列
void mrd_servo_drive_ics_double(MeridimUnion &a_meridim, ServoParam &a_sv, IcsHardSerialClass &a_servoL, IcsHardSerialClass &a_servoR) {
  for (int i = 0; i < a_sv.num_max; i++) {
    // L系統サーボの処理
    if (a_sv.ixl_mount[i]) {
      a_sv.ixl_tgt[i] = mrd_servo_process_ics(
          a_sv.ixl_id[i], a_meridim.sval[(i * 2) + 20], a_sv.ixl_tgt[i], a_sv.ixl_tgt_past[i],
          a_sv.ixl_trim[i], a_sv.ixl_cw[i], a_sv.ixl_err[i], a_sv.ixl_stat[i], a_servoL);
    }
    // R系統サーボの処理
    if (a_sv.ixr_mount[i]) {
      a_sv.ixr_tgt[i] = mrd_servo_process_ics(
          a_sv.ixr_id[i], a_meridim.sval[(i * 2) + 50], a_sv.ixr_tgt[i], a_sv.ixr_tgt_past[i],
          a_sv.ixr_trim[i], a_sv.ixr_cw[i], a_sv.ixr_err[i], a_sv.ixr_stat[i], a_servoR);
    }
#if defined(Meridian_TWIN_Tsy40)
    delayMicroseconds(1); // Teensyの場合には必要かも
#endif
  }
}

//==================================================================================================
//  gs2d によるICSサーボの処理
//  https://github.com/karakuri-products/gs2d
//==================================================================================================

/// @brief ICSサーボの実行処理を行う関数
/// @param a_servo_id サーボのインデックス番号
/// @param a_cmd サーボのコマンド
/// @param a_tgt サーボの目標位置
/// @param a_tgt_past 前回のサーボの目標位置
/// @param a_tgt_trim サーボの補正値
/// @param a_cw サーボの回転方向補正値
/// @param a_err_cnt サーボのエラーカウント
/// @param a_stat サーボのステータス
/// @param ics サーボクラスのインスタンス
// float mrd_servo_process_ics_gs2d(int a_servo_id, int a_cmd,
//                                  float a_tgt, float a_tgt_past, int a_trim,
//                                  int a_cw, int &a_err_cnt, uint16_t &a_stat,
//                                  IcsHardSerialClass &ics) {
//   int val_tmp = 0;
//   if (a_cmd == 1) { // コマンドが1ならPos指定
//     val_tmp = ics.setPos(a_servo_id, mrd_Deg2Krs(a_tgt, a_trim, a_cw));
//   } else { // コマンドが0等なら脱力して値を取得
//     val_tmp = ics.setFree(a_servo_id);
//   }
//   if (val_tmp == -1) { // サーボからの返信信号を受け取れなかった場合
//     val_tmp = mrd_Deg2Krs(a_tgt_past, a_trim, a_cw);
//     a_err_cnt++;
//     if (a_err_cnt >= SERVO_LOST_ERR_WAIT) { // 一定以上の連続エラーで通信不能とみなす
//       a_err_cnt = SERVO_LOST_ERR_WAIT;
//       a_stat = 1;
//     }
//   } else {
//     a_err_cnt = 0;
//     a_stat = 0;
//   }

//   return mrd_Krs2Deg(val_tmp, a_trim, a_cw);
// }

#endif // __MERIDIAN_SERVO_KONDO_ICS_H__
