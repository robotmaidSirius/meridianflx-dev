/**
 * @file common.hpp
 * @brief
 * @version 0.1.0
 * @date 2025-11-30
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULE_COMMON_HPP__
#define __MERIDIAN_MODULE_COMMON_HPP__
// ヘッダファイルの読み込み
#include "mrd_modules/common.hpp"
#include <meridim.hpp>
// ライブラリ導入
#include <Arduino.h>

#define IXL_MAX 15 // L系統の最大サーボ数. 標準は15.
#define IXR_MAX 15 // R系統の最大サーボ数. 標準は15.

enum PadType {   // リモコン種の列挙型(NONE, PC, MERIMOTE, BLUERETRO, SBDBT, KRR5FH)
  NONE = 0,      // リモコンなし
  PC = 0,        // PCからのPD入力情報を使用
  MERIMOTE = 1,  // MERIMOTE(未導入)
  BLUERETRO = 2, // BLUERETRO(未導入)
  SBDBT = 3,     // SBDBT(未導入)
  KRR5FH = 4,    // KRR5FH
  WIIMOTE = 5,   // WIIMOTE / WIIMOTE + Nunchuk
  WIIMOTE_C = 6, // WIIMOTE+Classic
};

// Meridim配列用の共用体の設定
typedef union {
  short sval[MRDM_LEN + 4];             // short型で90個の配列データを持つ
  unsigned short usval[MRDM_LEN + 2];   // 上記のunsigned short型
  uint8_t bval[+4];                     // byte型で180個の配列データを持つ
  uint8_t ubval[MERIDIM_BYTE_SIZE + 4]; // 上記のunsigned byte型
} MeridimUnion;

// サーボ用変数
struct ServoParam {
  // サーボの最大接続 (サーボ送受信のループ処理数）
  int num_max;

  // 各サーボのマウントありなし(config.hで設定)
  int ixl_mount[IXL_MAX]; // L系統
  int ixr_mount[IXR_MAX]; // R系統

  // 各サーボのコード上のインデックスに対し, 実際に呼び出すハードウェアのID番号(config.hで設定)
  int ixl_id[IXL_MAX]; // L系統の実サーボ呼び出しID番号
  int ixr_id[IXR_MAX]; // R系統の実サーボ呼び出しID番号

  // 各サーボの正逆方向補正用配列(config.hで設定)
  int ixl_cw[IXL_MAX]; // L系統
  int ixr_cw[IXR_MAX]; // R系統

  // 各サーボの直立ポーズトリム値(config.hで設定)
  float ixl_trim[IXL_MAX]; // L系統
  float ixr_trim[IXR_MAX]; // R系統

  // 各サーボのポジション値(degree)
  float ixl_tgt[IXL_MAX] = {0};      // L系統の目標値
  float ixr_tgt[IXR_MAX] = {0};      // R系統の目標値
  float ixl_tgt_past[IXL_MAX] = {0}; // L系統の前回の値
  float ixr_tgt_past[IXR_MAX] = {0}; // R系統の前回の値

  // サーボのエラーカウンタ配列
  int ixl_err[IXL_MAX] = {0}; // L系統
  int ixr_err[IXR_MAX] = {0}; // R系統

  // サーボのコンディションステータス配列
  uint16_t ixl_stat[IXL_MAX] = {0}; // L系統サーボのコンディションステータス配列
  uint16_t ixr_stat[IXR_MAX] = {0}; // R系統サーボのコンディションステータス配列
};

#endif // __MERIDIAN_MODULE_COMMON_HPP__
