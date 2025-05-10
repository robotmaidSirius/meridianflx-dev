#ifndef __MERIDIAN_CONFIG_H__
#define __MERIDIAN_CONFIG_H__

#include <Meridim90.hpp>

//==================================================================================================
//  MERIDIAN - LITE - ESP32の配線
//==================================================================================================
//
// ESP32devkitC  -  デバイス
//   3V3         -  BNO005 VIN
//   21          -  BNO005 SCL
//   22          -  BNO005 SDA
//   GND         -  BNO005 GND
//
//   4  EN       -  ICS変換基板 R系統 EN
//   16 RX       -  ICS変換基板 R系統 TX
//   17 TX       -  ICS変換基板 R系統 RX
//   5V          -  ICS変換基板 IOREF
//   GND         -  ICS変換基板 GND
//
//   33 EN       -  ICS変換基板 L系統 EN
//   32 RX       -  ICS変換基板 L系統 TX
//   27 TX       -  ICS変換基板 L系統 RX
//   5V          -  ICS変換基板 IOREF
//   GND         -  ICS変換基板 GND
//
//   27          -  SPI_MISO
//   23          -  SPI_MOSI
//   18          -  SPI_CSK
//   15          -  SPI_CS SD

//==================================================================================================
//  サーボIDとロボット部位、軸との対応表 (KHR-3HVの例)
//==================================================================================================
//
// ID    Parts/Axis ＜ICS_Left_Upper SIO1,SIO2＞
// [L00] 頭/ヨー
// [L01] 左肩/ピッチ
// [L02] 左肩/ロール
// [L03] 左肘/ヨー
// [L04] 左肘/ピッチ
// [L05] -
// ID    Parts/Axis ＜ICS_Left_Lower SIO3,SIO4＞
// [L06] 左股/ロール
// [L07] 左股/ピッチ
// [L08] 左膝/ピッチ
// [L09] 左足首/ピッチ
// [L10] 左足首/ロール
// ID    Parts/Axis  ＜ICS_Right_Upper SIO5,SIO6＞
// [R00] 腰/ヨー
// [R01] 右肩/ピッチ
// [R02] 右肩/ロール
// [R03] 右肘/ヨー
// [R04] 右肘/ピッチ
// [R05] -
// ID    Parts/Axis  ＜ICS_Right_Lower SIO7,SIO8＞
// [R06] 右股/ロール
// [R07] 右股/ピッチ
// [R08] 右膝/ピッチ
// [R09] 右足首/ピッチ
// [R10] 右足首/ロール

//==================================================================================================
//  MERIDIAN TWIN ESP32の配線
//==================================================================================================
//   ESP32devkitC  -  Teensy4.0 or PIN
//   [3.3v]        -> x
//   [EN]          -> x
//   [SENSOR_VP]   -> x
//   [SENSOR_VN]   -> x
//   [34]          -> x
//   [35]          -> x
//   [32]          -> x
//   [33]          -> x
//   [25]          -> x
//   [26]          -> x
//   [27]          -> x
//   [14]          -> SPI/SD_SCK (Teensy[13]) & SD_CLK (SD[5])
//   [12]          -> MISO (Teensy[12])
//   [GND1]        -> x
//   [13]          -> MOSI (Teensy[11])
//   [D2]          -> x
//   [D3]          -> x
//   [CMD]         -> x
//   [5V]          -> 5V

//   [GND3]        -> GND
//   [23]          -> x
//   [22]          -> x
//   [TXD0]        -> x
//   [RXD0]        -> x
//   [21]          -> x
//   [GND2]        -> x
//   [19]          -> x
//   [18]          -> x
//   [05]          -> x
//   [17]          -> x
//   [16]          -> x
//   [04]          -> x
//   [00]          -> x
//   [02]          -> x
//   [15]          -> SPI_CS (Teensy[10])
//   [D1]          -> x
//   [D0]          -> x
//   [CLK]         -> x

//-------------------------------------------------------------------------
//  各種設定
//-------------------------------------------------------------------------

// Meridimの基本設定
#define FRAME_DURATION 10  // 1フレームあたりの単位時間（単位ms）
#define CHARGE_TIME    200 // 起動時のコンデンサチャージ待機時間（単位ms）

// 各種ハードウェアのマウント有無
#define MOUNT_SD      1           // SDカードリーダーの有無 (0:なし, 1:あり)
#define MOUNT_IMUAHRS BNO055_AHRS // IMU/AHRSの搭載 NO_IMU, MPU6050_IMU, MPU9250_IMU, BNO055_AHRS
#define MOUNT_PAD     NONE        // ジョイパッドの搭載 NONE:0, PC:0, WIIMOTE:5, WIIMOTE_C:6

// 動作モード
#define MODE_ESP32_STANDALONE 0 // ESP32をボードに挿さず動作確認（0:NO, 1:YES）
#define MODE_UDP_RECEIVE      1 // PCからのデータ受信（0:OFF, 1:ON, 通常は1）
#define MODE_UDP_SEND         1 // PCへのデータ送信（0:OFF, 1:ON, 通常は1）

// Wifiの設定(SSID,パスワード等は別途keys.hで指定)
// #define MODE_FIXED_IP 0 // IPアドレスを固定するか（0:NO, 1:YES）
#define UDP_TIMEOUT 4 // UDPの待受タイムアウト（単位ms,推奨値0）

// EEPROMの設定
#define EEPROM_SIZE    540 // 使用するEEPROMのサイズ(バイト)
#define EEPROM_SET     0   // 起動時にEEPROMにconfig.hの内容をセット(mrd_set_eeprom)
#define EEPROM_PROTECT 0   // EEPROMの書き込み保護(0:保護しない, 1:書き込み禁止)
#define EEPROM_LOAD    0   // 起動時にEEPROMの内容を諸設定にロードする(未導入)
#define EEPROM_DUMP    1   // 起動時のEEPROM内容のダンプ表示
#define EEPROM_STYLE   Hex // 起動時のEEPROM内容のダンプ表示の書式(Bin,Hex,Dec)

// 動作チェックモード
#define CHECK_SD_RW     1 // 起動時のSDカードリーダーの読み書きチェック
#define CHECK_EEPROM_RW 0 // 起動時のEEPROMの動作チェック

// シリアルモニタリング
#define MONITOR_FRAME_DELAY       1    // シリアルモニタでフレーム遅延時間を表示（0:OFF, 1:ON）
#define MONITOR_FLOW              0    // シリアルモニタでフローを表示（0:OFF, 1:ON）
#define MONITOR_ERR_SERVO         0    // シリアルモニタでサーボエラーを表示（0:OFF, 1:ON）
#define MONITOR_ERR_ALL           0    // 全経路の受信エラー率を表示
#define MONITOR_SEQ               0    // シリアルモニタでシーケンス番号チェックを表示（0:OFF, 1:ON）
#define MONITOR_PAD               0    // シリアルモニタでリモコンのデータを表示（0:OFF, 1:ON）
#define MONITOR_SUPPRESS_DURATION 8000 // 起動直後のタイムアウトメッセージ抑制時間(単位ms)

// #define MONITOR_SUPPRESS_DURATION 20000 // 起動直後のタイムアウトメッセージ抑制時間(単位ms)

// I2C設定, I2Cセンサ関連設定
#define I2C0_SPEED       400000 // I2Cの速度（400kHz推奨）
#define IMUAHRS_INTERVAL 10     // IMU/AHRSのセンサの読み取り間隔(ms)
#define IMUAHRS_STOCK    4      // MPUで移動平均を取る際の元にする時系列データの個数
// #define I2C1_SPEED 100000  // I2Cの速度（100kHz推奨?）
// #define I2C1_MERIMOTE_ADDR 0x58 // MerimoteのI2Cアドレス

// SPI設定
#define SPI0_SPEED 6000000 // SPI通信の速度（6000000kHz推奨）

// PC接続関連設定
// #define SERIAL_PC_BPS 1000000 // PCとのシリアル速度（モニタリング表示用）(TWIN用)
#define SERIAL_PC_BPS     115200 // PCとのシリアル速度（モニタリング表示用）(Lite用)
#define SERIAL_PC_TIMEOUT 2000   // PCとのシリアル接続確立タイムアウト(ms)

// JOYPAD関連設定
#define PAD_INIT_TIMEOUT 10000 // 起動時のJOYPADの接続確立のタイムアウト(ms)
#define PAD_INTERVAL     10    // JOYPADのデータを読みに行くフレーム間隔 (※KRC-5FHでは4推奨)
#define PAD_BUTTON_MARGE 1     // 0:JOYPADのボタンデータをMeridim受信値に論理積, 1:Meridim受信値に論理和
#define PAD_GENERALIZE   1     // ジョイパッドの入力値をPS系に一般化する

// ピンアサイン
#define PIN_ERR_LED       25 // LED用 処理が時間内に収まっていない場合に点灯
#define PIN_EN_L          33 // サーボL系統のENピン
#define PIN_EN_R          4  // サーボR系統のENピン
#define PIN_CHIPSELECT_SD 15 // SDカード用のCSピン
#define PIN_I2C0_SDA      22 // I2CのSDAピン
#define PIN_I2C0_SCL      21 // I2CのSCLピン
#define PIN_LED_BT        23 // Bluetooth接続確認用ピン(点滅はペアリング,点灯でリンク確立)
// #define PIN_LED_BT        26 // Bluetooth接続確認用ピン(点滅はペアリング,点灯でリンク確立)

//-------------------------------------------------------------------------
// サーボ設定
//-------------------------------------------------------------------------

// コマンドサーボの種類
// 00: NOSERVO (マウントなし),            01: PWM_S1 (Single PWM)[WIP]
// 11: PCA9685 (I2C_PCA9685toPWM)[WIP], 21: FTBRSX (FUTABA_RSxTTL)[WIP]
// 31: DXL1 (DYNAMIXEL 1.0)[WIP],       32: DXL2 (DYNAMIXEL 2.0)[WIP]
// 43: KOICS3 (KONDO_ICS 3.5 / 3.6),    44: KOPMX (KONDO_PMX)[WIP]
// 51: JRXBUS (JRPROPO_XBUS)[WIP]
// 61: FTCSTS (FEETECH_STS)[WIP],       62: FTCSCS (FEETECH_SCS)[WIP]
#define MOUNT_SERVO_TYPE_L 43 // L系統のコマンドサーボの種類
#define MOUNT_SERVO_TYPE_R 43 // R系統のコマンドサーボの種類

// サーボ関連設定
#define SERVO_BAUDRATE_L    1250000 // L系統のICSサーボの通信速度bps
#define SERVO_BAUDRATE_R    1250000 // R系統のICSサーボの通信速度bps
#define SERVO_TIMEOUT_L     2       // L系統のICS返信待ちのタイムアウト時間
#define SERVO_TIMEOUT_R     2       // R系統のICS返信待ちのタイムアウト時間
#define SERVO_LOST_ERR_WAIT 6       // 連続何フレームサーボ信号をロストしたら異常とするか

// 各サーボ系統の最大サーボマウント数
#define IXL_MAX 15 // L系統の最大サーボ数. 標準は15.
#define IXR_MAX 15 // R系統の最大サーボ数. 標準は15.

// L系統のサーボのマウントの設定
// 00: NOSERVO (マウントなし),            01: PWM_S1 (Single PWM)[WIP]
// 11: PCA9685 (I2C_PCA9685toPWM)[WIP], 21: FTBRSX (FUTABA_RSxTTL)[WIP]
// 31: DXL1 (DYNAMIXEL 1.0)[WIP],       32: DXL2 (DYNAMIXEL 2.0)[WIP]
// 43: KOICS3 (KONDO_ICS 3.5 / 3.6),    44: KOPMX (KONDO_PMX)[WIP]
// 51: JRXBUS (JRPROPO_XBUS)[WIP]
// 61: FTCSTS (FEETECH_STS)[WIP],       62: FTCSCS (FEETECH_SCS)[WIP]
int IXL_MT[IXL_MAX] = {
    // L系統のマウント状態
    43, // [00]頭ヨー
    43, // [01]左肩ピッチ
    43, // [02]左肩ロール
    43, // [03]左肘ヨー
    43, // [04]左肘ピッチ
    43, // [05]左股ヨー
    43, // [06]左股ロール
    43, // [07]左股ピッチ
    43, // [08]左膝ピッチ
    43, // [09]左足首ピッチ
    43, // [10]左足首ロール
    0,  // [11]追加サーボ用
    0,  // [12]追加サーボ用
    0,  // [13]追加サーボ用
    0   // [14]追加サーボ用
};

// R系統のサーボのマウントの設定
// 00: NOSERVO (マウントなし),            01: PWM_S1 (Single PWM)[WIP]
// 11: PCA9685 (I2C_PCA9685toPWM)[WIP], 21: FTBRSX (FUTABA_RSxTTL)[WIP]
// 31: DXL1 (DYNAMIXEL 1.0)[WIP],       32: DXL2 (DYNAMIXEL 2.0)[WIP]
// 43: KOICS3 (KONDO_ICS 3.5 / 3.6),    44: KOPMX (KONDO_PMX)[WIP]
// 51: JRXBUS (JRPROPO_XBUS)[WIP]
// 61: FTCSTS (FEETECH_STS)[WIP],       62: FTCSCS (FEETECH_SCS)[WIP]
int IXR_MT[IXR_MAX] = {
    // R系統のマウント状態
    43, // [00]腰ヨー
    43, // [01]右肩ピッチ
    43, // [02]右肩ロール
    43, // [03]右肘ヨー
    43, // [04]右肘ピッチ
    43, // [05]右股ヨー
    43, // [06]右股ロール
    43, // [07]右股ピッチ
    43, // [08]右膝ピッチ
    43, // [09]右足首ピッチ
    43, // [10]右足首ロール
    0,  // [11]追加サーボ用
    0,  // [12]追加サーボ用
    0,  // [13]追加サーボ用
    0   // [14]追加サーボ用
};

// L系統のコード上のサーボIndexに対し, 実際に呼び出すハードウェアのID番号
int IXL_ID[IXL_MAX] = {
    0,  // [00]頭ヨー
    1,  // [01]左肩ピッチ
    2,  // [02]左肩ロール
    3,  // [03]左肘ヨー
    4,  // [04]左肘ピッチ
    5,  // [05]左股ヨー
    6,  // [06]左股ロール
    7,  // [07]左股ピッチ
    8,  // [08]左膝ピッチ
    9,  // [09]左足首ピッチ
    10, // [10]左足首ロール
    11, // [11]追加サーボ用
    12, // [12]追加サーボ用
    13, // [13]追加サーボ用
    14  // [14]追加サーボ用
};

// R系統のコード上のサーボIndexに対し, 実際に呼び出すハードウェアのID番号
int IXR_ID[IXR_MAX] = {
    0,  // [00]腰ヨー
    1,  // [01]右肩ピッチ
    2,  // [02]右肩ロール
    3,  // [03]右肘ヨー
    4,  // [04]右肘ピッチ
    5,  // [05]右股ヨー
    6,  // [06]右股ロール
    7,  // [07]右股ピッチ
    8,  // [08]右膝ピッチ
    9,  // [09]右足首ピッチ
    10, // [10]右足首ロール
    11, // [11]追加サーボ用
    12, // [12]追加サーボ用
    13, // [13]追加サーボ用
    14  // [14]追加サーボ用
};

// L系統のサーボ回転方向補正(1:変更なし, -1:逆転)
int IXL_CW[IXL_MAX] = {
    1, // [00]頭ヨー
    1, // [01]左肩ピッチ
    1, // [02]左肩ロール
    1, // [03]左肘ヨー
    1, // [04]左肘ピッチ
    1, // [05]左股ヨー
    1, // [06]左股ロール
    1, // [07]左股ピッチ
    1, // [08]左膝ピッチ
    1, // [09]左足首ピッチ
    1, // [10]左足首ロール
    1, // [11]追加サーボ用
    1, // [12]追加サーボ用
    1, // [13]追加サーボ用
    1  // [14]追加サーボ用
};

// R系統のサーボ回転方向補正(1:変更なし, -1:逆転)
int IXR_CW[IXR_MAX] = {
    // R系統の正転逆転
    1, // [00]腰ヨー
    1, // [01]右肩ピッチ
    1, // [02]右肩ロール
    1, // [03]右肘ヨー
    1, // [04]右肘ピッチ
    1, // [05]右股ヨー
    1, // [06]右股ロール
    1, // [07]右股ピッチ
    1, // [08]右膝ピッチ
    1, // [09]右足首ピッチ
    1, // [10]右足首ロール
    1, // [11]追加サーボ用
    1, // [12]追加サーボ用
    1, // [13]追加サーボ用
    1  // [14]追加サーボ用
};

// L系統のトリム値(degree)
float IDL_TRIM[IXL_MAX] = {
    0,      // [00]頭ヨー
    -2.36,  // [01]左肩ピッチ
    -91.13, // [02]左肩ロール
    0,      // [03]左肘ヨー
    89.98,  // [04]左肘ピッチ
    0,      // [05]左股ヨー
    0,      // [06]左股ロール
    -1.35,  // [07]左股ピッチ
    -58.05, // [08]左膝ピッチ
    -20.25, // [09]左足首ピッチ
    -0.68,  // [10]左足首ロール
    0,      // [11]追加サーボ用
    0,      // [12]追加サーボ用
    0,      // [13]追加サーボ用
    0       // [14]追加サーボ用
};

// R系統のトリム値(degree)
float IDR_TRIM[IXR_MAX] = {
    0,      // [00]腰ヨー
    0,      // [01]右肩ピッチ
    -89.44, // [02]右肩ロール
    0,      // [03]右肘ヨー
    89.98,  // [04]右肘ピッチ
    0,      // [05]右股ヨー
    1.69,   // [06]右股ロール
    -3.38,  // [07]右股ピッチ
    -57.38, // [08]右膝ピッチ
    -20.25, // [09]右足首ピッチ
    -2.36,  // [10]右足首ロール
    0,      // [11]追加サーボ用
    0,      // [12]追加サーボ用
    0,      // [13]追加サーボ用
    0,      // [14]追加サーボ用
};

#endif // __MERIDIAN_CONFIG_H__
