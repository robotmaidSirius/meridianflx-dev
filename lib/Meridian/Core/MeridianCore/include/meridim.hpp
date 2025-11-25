/**
 * @file Meridim.hpp
 * @brief Meridianで定義されているMeridim型
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_CORE_MERIDIM_MERIDIM_HPP__
#define __MERIDIAN_CORE_MERIDIM_MERIDIM_HPP__

#include <stdint.h>
#define Meridim_SEQUENTIAL_MAX (0xEA60) // シーケンス番号の最大値: Defaultは60000

#define Meridim_DATA_LEN  (90)                   // Meridimのデータ数
#define Meridim_BYTE_SIZE (Meridim_DATA_LEN * 2) // Meridimのバイト数

#ifndef Meridim_SERVO_NUM
#define Meridim_SERVO_NUM (30) /**< 接続するサーボの数 */
#endif

// ユーザーデータのサイズを定義
//   (データ数) - (マスターコマンドからフレーム数までのデータ数) - (サーボ数) - (エラーコードとチェックサム)
#define Meridim_USER_DATA_SIZE (Meridim_DATA_LEN - 40 - Meridim_SERVO_NUM - 2) /**< ユーザーデータサイズを設定する */

namespace meridian {

struct MeridimVector {
  int16_t x;
  int16_t y;
  int16_t z;
};
struct MeridimRPY {
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
};
struct MeridimServo {
  uint8_t id;
  uint8_t cmd;
  int16_t value;
};
struct MeridimController {
  int16_t buttons;  ///! リモコンの基本ボタン値
  int16_t stick_l;  ///! リモコンの左スティックアナログ値
  int16_t stick_r;  ///! リモコンの右スティックアナログ値
  uint8_t analog_l; ///! リモコンのL2ボタンアナログ値
  uint8_t analog_r; ///! リモコンのR2ボタンアナログ値
};
struct MeridimInput {
  MeridimVector accelerator;  ///! 加速度センサ値
  MeridimVector gyroscope;    ///! 加速度センサ値
  MeridimVector magnetometer; ///! 加速度センサ値
  int16_t temperature;        ///! 温度センサ値
  MeridimRPY dmp;             ///! DMP推定
  MeridimController control;  ///! リモコンの基本ボタン値
};
struct MeridimUserdata {
  uint8_t motion_frames;  ///! モーション設定のフレーム数
  uint8_t stop_frames_ms; ///! ボード停止時のフレーム数

  MeridimServo servo[Meridim_SERVO_NUM];   ///! サーボのコマンドと値
  int16_t options[Meridim_USER_DATA_SIZE]; ///! ユーザー定義用
};
struct Meridim {
  uint16_t master_command;  ///!	マスターコマンド
  uint16_t sequential;      ///! シーケンス番号
  MeridimInput input_data;  ///! リモコンの基本ボタン値
  MeridimUserdata userdata; ///! ユーザー定義用

  uint16_t err;      ///! ERROR CODE
  uint16_t checksum; ///! CHECK SUM
};

void mrd_countup(Meridim &a_meridim) {
  if (Meridim_SEQUENTIAL_MAX <= a_meridim.sequential) {
    a_meridim.sequential = 0;
  } else {
    a_meridim.sequential++;
  }
}

void set_Meridim(Meridim &a_meridim, uint8_t a_data[], int size) {
  if (Meridim_BYTE_SIZE > size) {
    a_meridim.master_command = (uint16_t)((a_data[1] << 8) | a_data[0]);
    a_meridim.sequential = (uint16_t)((a_data[3] << 8) | a_data[2]);
    a_meridim.input_data.accelerator.x = (int16_t)((a_data[5] << 8) | a_data[4]);
    a_meridim.input_data.accelerator.y = (int16_t)((a_data[7] << 8) | a_data[6]);
    a_meridim.input_data.accelerator.z = (int16_t)((a_data[9] << 8) | a_data[8]);
    a_meridim.input_data.gyroscope.x = (int16_t)((a_data[11] << 8) | a_data[10]);
    a_meridim.input_data.gyroscope.y = (int16_t)((a_data[13] << 8) | a_data[12]);
    a_meridim.input_data.gyroscope.z = (int16_t)((a_data[15] << 8) | a_data[14]);
    a_meridim.input_data.magnetometer.x = (int16_t)((a_data[17] << 8) | a_data[16]);
    a_meridim.input_data.magnetometer.y = (int16_t)((a_data[19] << 8) | a_data[18]);
    a_meridim.input_data.magnetometer.z = (int16_t)((a_data[21] << 8) | a_data[20]);
    a_meridim.input_data.temperature = (int16_t)((a_data[23] << 8) | a_data[22]);
    a_meridim.input_data.dmp.roll = (int16_t)((a_data[25] << 8) | a_data[24]);
    a_meridim.input_data.dmp.pitch = (int16_t)((a_data[27] << 8) | a_data[26]);
    a_meridim.input_data.dmp.yaw = (int16_t)((a_data[29] << 8) | a_data[28]);
    a_meridim.input_data.control.buttons = (int16_t)((a_data[31] << 8) | a_data[30]);
    a_meridim.input_data.control.stick_l = (int16_t)((a_data[33] << 8) | a_data[32]);
    a_meridim.input_data.control.stick_r = (int16_t)((a_data[35] << 8) | a_data[34]);
    a_meridim.input_data.control.analog_l = a_data[36];
    a_meridim.input_data.control.analog_r = a_data[37];
    a_meridim.userdata.motion_frames = a_data[38] > 4;
    a_meridim.userdata.stop_frames_ms = a_data[38] & 0x0F;
    for (int i = 0; i < Meridim_SERVO_NUM; i++) {
      a_meridim.userdata.servo[i].id = a_data[39 + (i * 3)] > 4;
      a_meridim.userdata.servo[i].cmd = a_data[39 + (i * 3)] & 0x0F;
      a_meridim.userdata.servo[i].value = (int16_t)((a_data[41 + (i * 3)] << 8) | a_data[40 + (i * 3)]);
    }
    for (int i = 0; i < Meridim_USER_DATA_SIZE; i++) {
      a_meridim.userdata.options[i] = (int16_t)((a_data[39 + (Meridim_SERVO_NUM * 3) + (i * 3)] << 8) |
                                                a_data[40 + (Meridim_SERVO_NUM * 3) + (i * 3)]);
    }
    a_meridim.err = (a_data[39 + (Meridim_SERVO_NUM * 3) + (Meridim_USER_DATA_SIZE * 2)] << 8) |
                    a_data[40 + (Meridim_SERVO_NUM * 3) + (Meridim_USER_DATA_SIZE * 2)];
    a_meridim.checksum = (a_data[41 + (Meridim_SERVO_NUM * 3) + (Meridim_USER_DATA_SIZE * 2)] << 8) |
                         a_data[42 + (Meridim_SERVO_NUM * 3) + (Meridim_USER_DATA_SIZE * 2)];
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Meridimの基本設定
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MRDM_LEN 90 // Meridim配列の長さ設定（デフォルトは90）

// エラービット MRD_ERR_CODEの上位8bit分
#define ERRBIT_15_ESP_PC       15 // ESP32 → PC のUDP受信エラー (0:エラーなし、1:エラー検出)
#define ERRBIT_14_PC_ESP       14 // PC → ESP32 のUDP受信エラー
#define ERRBIT_13_ESP_TSY      13 // ESP32 → TeensyのSPI受信エラー
#define ERRBIT_12_TSY_ESP      12 // Teensy → ESP32 のSPI受信エラー
#define ERRBIT_11_BOARD_DELAY  11 // Teensy or ESP32の処理ディレイ (末端で捕捉)
#define ERRBIT_10_UDP_ESP_SKIP 10 // PC → ESP32 のUDPフレームスキップエラー
#define ERRBIT_9_BOARD_SKIP    9  // PC → ESP32 → Teensy のフレームスキップエラー(末端で捕捉)
#define ERRBIT_8_PC_SKIP       8  // Teensy → ESP32 → PC のフレームスキップエラー(末端で捕捉)
enum Meridim_ERRBIT {
  ESP_PC = 0x01 << 15,       ///! ESP32 → PC のUDP受信エラー (0:エラーなし、1:エラー検出)
  PC_ESP = 0x01 << 14,       ///! PC → ESP32 のUDP受信エラー
  ESP_TSY = 0x01 << 13,      ///! ESP32 → TeensyのSPI受信エラー
  TSY_ESP = 0x01 << 12,      ///! Teensy → ESP32 のSPI受信エラー
  BOARD_DELAY = 0x01 << 11,  ///! Teensy or ESP32の処理ディレイ (末端で捕捉)
  UDP_ESP_SKIP = 0x01 << 10, ///! PC → ESP32 のUDPフレームスキップエラー
  BOARD_SKIP = 0x01 << 9,    ///! PC → ESP32 → Teensy のフレームスキップエラー(末端で捕捉)
  PC_SKIP = 0x01 << 8,       ///! Teensy → ESP32 → PC のフレームスキップエラー(末端で捕捉)

};

//-------------------------------------------------------------------------
//---- Meridim 配列アクセス対応キー  --------------------------------------
//-------------------------------------------------------------------------
enum Meridim_KEY {
  MASTER = 0,         // マスターコマンド
  SEQ = 2,            // シーケンス番号
  ACC_X = 4,          // 加速度センサX値
  ACC_Y = 6,          // 加速度センサY値
  ACC_Z = 8,          // 加速度センサZ値
  GYRO_X = 10,        // ジャイロセンサX値
  GYRO_Y = 12,        // ジャイロセンサY値
  GYRO_Z = 14,        // ジャイロセンサZ値
  MAG_X = 16,         // 磁気コンパスX値
  MAG_Y = 18,         // 磁気コンパスY値
  MAG_Z = 20,         // 磁気コンパスZ値
  TEMP = 22,          // 温度センサ値
  DIR_ROLL = 24,      // DMP推定ロール方向値
  DIR_PITCH = 26,     // DMP推定ピッチ方向値
  DIR_YAW = 28,       // DMP推定ヨー方向値
  PAD_BUTTONS = 30,   // リモコンの基本ボタン値
  PAD_STICK_L = 32,   // リモコンの左スティックアナログ値
  PAD_STICK_R = 34,   // リモコンの右スティックアナログ値
  PAD_L2R2VAL = 36,   // リモコンのL2R2ボタンアナログ値
  MOTION_FRAMES = 38, // モーション設定のフレーム数
  STOP_FRAMES = 38,   // ボード停止時のフレーム数(MCMD_BOARD_STOP_DURINGで指定)
  SERVO_CMD = 40,
};

#define MRD_ERR_CODE (MRDM_LEN - 2) // エラーコード (MRDM_LEN - 2)
#define MRD_CKSM     (MRDM_LEN - 1) // チェックサム (MRDM_LEN - 1)

//-------------------------------------------------------------------------
//  固定値, マスターコマンド定義
//-------------------------------------------------------------------------
enum MASTER_COMMAND {
  MCMD_TORQUE_ALL_OFF = 0,             // すべてのサーボトルクをオフにする（脱力）
  MCMD_DUMMY_DATA = -32768,            // SPI送受信用のダミーデータ判定用
  MCMD_TEST_VALUE = -32767,            // テスト用の仮設変数
  MCMD_SENSOR_YAW_CALIB = 10002,       // センサの推定ヨー軸を現在値センターとしてリセット
  MCMD_SENSOR_ALL_CALIB = 10003,       // センサの3軸について現在値を原点としてリセット
  MCMD_ERR_CLEAR_SERVO_ID = 10004,     // 通信エラーのサーボのIDをクリア(MRD_ERR_l)
  MCMD_BOARD_TRANSMIT_ACTIVE = 10005,  // ボードが定刻で送信を行うモード（PC側が受信待ち）
  MCMD_BOARD_TRANSMIT_PASSIVE = 10006, // ボードが受信を待ち返信するモード（PC側が定刻送信）
  MCMD_FRAMETIMER_RESET = 10007,       // フレームタイマーを現在時刻にリセット
  MCMD_BOARD_STOP_DURING = 10008,      // ボードの末端処理を[MRD_STOP_FRAMES]ミリ秒止める
  MCMD_EEPROM_ENTER_WRITE = 10009,     // EEPROM書き込みモードのスタート
  MCMD_EEPROM_EXIT_WRITE = 10010,      // EEPROM書き込みモードの終了
  MCMD_EEPROM_ENTER_READ = 10011,      // EEPROM読み出しモードのスタート
  MCMD_EEPROM_EXIT_READ = 10012,       // EEPROM読み出しモードの終了
  MCMD_SDCARD_ENTER_WRITE = 10013,     // SDCARD書き込みモードのスタート
  MCMD_SDCARD_EXIT_WRITE = 10014,      // SDCARD書き込みモードの終了
  MCMD_SDCARD_ENTER_READ = 10015,      // SDCARD読み出しモードのスタート
  MCMD_SDCARD_EXIT_READ = 10016,       // SDCARD読み出しモードの終了
  MCMD_EEPROM_SAVE_TRIM = 10101,       // 現在の姿勢をトリム値としてサーボに書き込む
  MCMD_EEPROM_LOAD_TRIM = 10102,       // EEPROMのトリム値をサーボに反映する
  MCMD_NAK = 32766,                    // コマンド実行の失敗を応答
  MCMD_ACK = 32767,                    // コマンド実行の成功を応答
};

} // namespace meridian

using namespace meridian;

#endif // __MERIDIAN_CORE_MERIDIM_MERIDIM_HPP__
