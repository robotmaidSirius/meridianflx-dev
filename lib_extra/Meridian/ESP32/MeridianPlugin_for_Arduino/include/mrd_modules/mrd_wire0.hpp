/**
 * @file mrd_wire0.h
 * @brief
 * @version 0.1.0
 * @date 2025-11-29
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULE_MRD_WIRE0_HPP__
#define __MERIDIAN_MODULE_MRD_WIRE0_HPP__
// ヘッダファイルの読み込み
#include "mrd_modules/common.hpp"

// ライブラリ導入

#include <Adafruit_BNO055.h>            // BNO055用
#include <MPU6050_6Axis_MotionApps20.h> // MPU6050用

#define IMUAHRS_STOCK    4           // MPUで移動平均を取る際の元にする時系列データの個数
#define IMUAHRS_INTERVAL 10          // IMU/AHRSのセンサの読み取り間隔(ms)
#define MOUNT_IMUAHRS    BNO055_AHRS // IMU/AHRSの搭載 NO_IMU, MPU6050_IMU, MPU9250_IMU, BNO055_AHRS

enum ImuAhrsType { // 6軸9軸センサ種の列挙型(NO_IMU, MPU6050_IMU, MPU9250_IMU, BNO055_AHRS)
  NO_IMU = 0,      // IMU/AHRS なし.
  MPU6050_IMU = 1, // MPU6050
  MPU9250_IMU = 2, // MPU9250(未設定)
  BNO055_AHRS = 3  // BNO055
};
// 6軸or9軸センサーの値
struct AhrsValue {
  Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); // BNO055のインスタンス

  MPU6050 mpu6050;        // MPU6050のインスタンス
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [roll, pitch, yaw]   roll/pitch/yaw container and gravity vector
  float yaw_origin = 0;   // ヨー軸の補正センター値
  float yaw_source = 0;   // ヨー軸のソースデータ保持用

  float read[16]; // mpuからの読み込んだ一次データacc_x,y,z,gyro_x,y,z,mag_x,y,z,gr_x,y,z,rpy_r,p,y,temp

  float zeros[16] = {0};               // リセット用
  float ave_data[16];                  // 上記の移動平均値を入れる
  float result[16];                    // 加工後の最新のmpuデータ（二次データ）
  float stock_data[IMUAHRS_STOCK][16]; // 上記の移動平均値計算用のデータストック
  int stock_count = 0;                 // 上記の移動平均値計算用のデータストックを輪番させる時の変数
  VectorInt16 aa;                      // [x, y, z]            加速度センサの測定値
  VectorInt16 gyro;                    // [x, y, z]            角速度センサの測定値
  VectorInt16 mag;                     // [x, y, z]            磁力センサの測定値
  long temperature;                    // センサの温度測定値
};
AhrsValue ahrs;

// ライブラリ導入
#include <Arduino.h>
#include <Wire.h>

//==================================================================================================
//  I2C wire0 関連の処理
//==================================================================================================

//------------------------------------------------------------------------------------
//  初期設定
//------------------------------------------------------------------------------------

/// @brief Wire0 I2C通信を初期化し, 指定されたクロック速度で設定する.
/// @param a_i2c0_speed I2C通信のクロック速度です.
/// @param a_pinSDA SDAのピン番号. 下記と合わせて省略可.
/// @param a_pinSCL SCLのピン番号. 上記と合わせて省略可.
bool mrd_wire0_init_i2c(int a_i2c0_speed, int a_pinSDA = -1, int a_pinSCL = -1) {
  Serial.print("Initializing wire0 I2C... ");
  if (a_pinSDA == -1 && a_pinSCL == -1) {
    Wire.begin();
  } else {
    Wire.begin(a_pinSDA, a_pinSCL);
  }
  Wire.setClock(a_i2c0_speed);
  return true;
}

/// @brief MPU6050センサーのDMP（デジタルモーションプロセッサ）を初期化し,
///        ジャイロスコープと加速度センサーのオフセットを設定する.
/// @param a_ahrs AHRSの値を保持する構造体.
/// @return DMPの初期化が成功した場合はtrue, 失敗した場合はfalseを返す.
bool mrd_wire0_init_mpu6050_dmp(AhrsValue &a_ahrs) {
  a_ahrs.mpu6050.initialize();
  a_ahrs.devStatus = a_ahrs.mpu6050.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  a_ahrs.mpu6050.setXAccelOffset(-1745);
  a_ahrs.mpu6050.setYAccelOffset(-1034);
  a_ahrs.mpu6050.setZAccelOffset(966);
  a_ahrs.mpu6050.setXGyroOffset(176);
  a_ahrs.mpu6050.setYGyroOffset(-6);
  a_ahrs.mpu6050.setZGyroOffset(-25);

  // make sure it worked (returns 0 if so)
  if (a_ahrs.devStatus == 0) {
    a_ahrs.mpu6050.CalibrateAccel(6);
    a_ahrs.mpu6050.CalibrateGyro(6);
    a_ahrs.mpu6050.setDMPEnabled(true);
    a_ahrs.packetSize = a_ahrs.mpu6050.dmpGetFIFOPacketSize();
    Serial.println("MPU6050 OK.");
    return true;
  }
  Serial.println("IMU/AHRS DMP Initialization FAILED!");
  return false;
}

/// @brief BNO055センサーの初期化を試みます.
/// @param a_ahrs AHRSの値を保持する構造体.
/// @return BNO055センサーの初期化が成功した場合はtrue, それ以外の場合はfalseを返す.
///         現在, この関数は常にfalseを返すように設定されています.
bool mrd_wire0_init_bno055(AhrsValue &a_ahrs) {
  if (!a_ahrs.bno.begin()) {
    Serial.println("No BNO055 detected ... Check your wiring or I2C ADDR!");
    return false;
  } else {
    Serial.println("BNO055 mounted.");
    delay(50);
    a_ahrs.bno.setExtCrystalUse(false);
    delay(10);
    return true;
  }
  // データの取得はセンサー用スレッドで実行
}

/// @brief 指定されたIMU/AHRSタイプに応じて適切なセンサの初期化を行います.
/// @param a_imuahrs_type 使用するセンサのタイプを示す列挙型です（MPU6050, MPU9250, BNO055）.
/// @param a_i2c0_speed I2C通信のクロック速度です.
/// @param a_ahrs AHRSの値を保持する構造体.
/// @param a_pinSDA SDAのピン番号.下記と合わせて省略可.
/// @param a_pinSCL SCLのピン番号.上記と合わせて省略可.
/// @return センサが正しく初期化された場合はtrueを, そうでない場合はfalseを返す.
bool mrd_wire0_setup(ImuAhrsType a_imuahrs_type, int a_i2c0_speed, AhrsValue &a_ahrs,
                     int a_pinSDA = -1, int a_pinSCL = -1) {
  if (a_imuahrs_type > 0) // 何らかのセンサを搭載
  {
    if (a_pinSDA == -1 && a_pinSCL == -1) {
      mrd_wire0_init_i2c(a_i2c0_speed);
    } else {
      mrd_wire0_init_i2c(a_i2c0_speed, a_pinSDA, a_pinSCL);
    }
  }

  if (a_imuahrs_type == MPU6050_IMU) // MPU6050
  {
    return mrd_wire0_init_mpu6050_dmp(a_ahrs);
  } else if (a_imuahrs_type == MPU9250_IMU) // MPU9250の場合
  {
    // mrd_wire_init_mpu9250_dmp(a_ahrs)
    return false;
  } else if (a_imuahrs_type == BNO055_AHRS) // BNO055の場合
  {
    return mrd_wire0_init_bno055(a_ahrs);
  }

  Serial.println("No IMU/AHRS sensor mounted.");
  return false;
}

//------------------------------------------------------------------------------------
//  センサデータの取得処理
//------------------------------------------------------------------------------------

/// @brief bno055からI2C経由でデータを読み取るスレッド用関数. IMUAHRS_INTERVALの間隔で実行する.
void mrd_wire0_Core0_bno055_r(void *args) {
  while (1) {
    // 加速度センサ値の取得と表示 - VECTOR_ACCELEROMETER - m/s^2
    imu::Vector<3> accelerometer = ahrs.bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    ahrs.read[0] = (float)accelerometer.x();
    ahrs.read[1] = (float)accelerometer.y();
    ahrs.read[2] = (float)accelerometer.z();

    // ジャイロセンサ値の取得 - VECTOR_GYROSCOPE - rad/s
    imu::Vector<3> gyroscope = ahrs.bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    ahrs.read[3] = gyroscope.x();
    ahrs.read[4] = gyroscope.y();
    ahrs.read[5] = gyroscope.z();

    // 磁力センサ値の取得と表示  - VECTOR_MAGNETOMETER - uT
    imu::Vector<3> magnetometer = ahrs.bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    ahrs.read[6] = magnetometer.x();
    ahrs.read[7] = magnetometer.y();
    ahrs.read[8] = magnetometer.z();

    // センサフュージョンによる方向推定値の取得と表示 - VECTOR_EULER - degrees
    imu::Vector<3> euler = ahrs.bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    ahrs.read[12] = euler.y();                   // DMP_ROLL推定値
    ahrs.read[13] = euler.z();                   // DMP_PITCH推定値
    ahrs.yaw_source = euler.x();                 // ヨー軸のソースデータ保持
    float yaw_tmp = euler.x() - ahrs.yaw_origin; // DMP_YAW推定値
    if (yaw_tmp >= 180) {
      yaw_tmp = yaw_tmp - 360;
    } else if (yaw_tmp < -180) {
      yaw_tmp = yaw_tmp + 360;
    }
    ahrs.read[14] = yaw_tmp; // DMP_YAW推定値
    ahrs.ypr[0] = ahrs.read[14];
    ahrs.ypr[1] = ahrs.read[13];
    ahrs.ypr[2] = ahrs.read[12];

    // センサフュージョンの方向推定値のクオータニオン
    // imu::Quaternion quat = bno.getQuat();

    // Serial.print("qW: ");
    // Serial.print(quat.w(), 4);
    // Serial.print(" qX: ");
    // Serial.print(quat.x(), 4);
    // Serial.print(" qY: ");
    // Serial.print(quat.y(), 4);
    // Serial.print(" qZ: ");
    // Serial.println(quat.z(), 4);

    // キャリブレーションのステータスの取得と表示
    // uint8_t system, gyro, accel, mag = 0;
    // bno.getCalibration(&system, &gyro, &accel, &mag);
    // Serial.print("CALIB Sys:");
    // Serial.print(system, DEC);
    // Serial.print(", Gy");
    // Serial.print(gyro, DEC);
    // Serial.print(", Ac");
    // Serial.print(accel, DEC);
    // Serial.print(", Mg");
    // Serial.println(mag, DEC);

    delay(IMUAHRS_INTERVAL);
  }
}

/// @brief AHRSセンサーからI2C経由でデータを読み取る関数.
/// MPU6050, MPU9250を想定していますが, MPU9250は未実装.
/// 各データは`ahrs.read`配列に格納され, 利用可能な場合は`ahrs.result`にコピーされる.
bool mrd_wire0_read_ahrs_i2c(AhrsValue &a_ahrs, bool imuahrs_available) { // ※wireTimer0.beginの引数のためvoid必須

  if (MOUNT_IMUAHRS == MPU6050_IMU) {                                // MPU6050
    if (a_ahrs.mpu6050.dmpGetCurrentFIFOPacket(a_ahrs.fifoBuffer)) { // Get new data
      a_ahrs.mpu6050.dmpGetQuaternion(&a_ahrs.q, a_ahrs.fifoBuffer);
      a_ahrs.mpu6050.dmpGetGravity(&a_ahrs.gravity, &a_ahrs.q);
      a_ahrs.mpu6050.dmpGetYawPitchRoll(a_ahrs.ypr, &a_ahrs.q, &a_ahrs.gravity);

      // acceleration values
      a_ahrs.mpu6050.dmpGetAccel(&a_ahrs.aa, a_ahrs.fifoBuffer);
      a_ahrs.read[0] = (float)a_ahrs.aa.x;
      a_ahrs.read[1] = (float)a_ahrs.aa.y;
      a_ahrs.read[2] = (float)a_ahrs.aa.z;

      // gyro values
      a_ahrs.mpu6050.dmpGetGyro(&a_ahrs.gyro, a_ahrs.fifoBuffer);
      a_ahrs.read[3] = (float)a_ahrs.gyro.x;
      a_ahrs.read[4] = (float)a_ahrs.gyro.y;
      a_ahrs.read[5] = (float)a_ahrs.gyro.z;

      // magnetic field values
      a_ahrs.read[6] = (float)a_ahrs.mag.x;
      a_ahrs.read[7] = (float)a_ahrs.mag.y;
      a_ahrs.read[8] = (float)a_ahrs.mag.z;

      // Estimated gravity DMP value.
      a_ahrs.read[9] = a_ahrs.gravity.x;
      a_ahrs.read[10] = a_ahrs.gravity.y;
      a_ahrs.read[11] = a_ahrs.gravity.z;

      // Estimated heading value using DMP.
      a_ahrs.read[12] = a_ahrs.ypr[2] * 180 / M_PI;                       // Estimated DMP_ROLL
      a_ahrs.read[13] = a_ahrs.ypr[1] * 180 / M_PI;                       // Estimated DMP_PITCH
      a_ahrs.read[14] = (a_ahrs.ypr[0] * 180 / M_PI) - a_ahrs.yaw_origin; // Estimated DMP_YAW

      // Temperature
      a_ahrs.read[15] = 0; // Not implemented.

      if (imuahrs_available) {
        memcpy(a_ahrs.result, a_ahrs.read, sizeof(float) * 16);
      }
      return true;
    } else {
      return false;
    }
  } else if (MOUNT_IMUAHRS == MPU9250_IMU) { // MPU9250
    return false;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------------
//  meriput
//------------------------------------------------------------------------------------
short mrd_float2HfShort(float val) {
  int _x = round(val * 100);
  if (_x > 32766) {
    _x = 32767;
  } else if (_x < -32766) {
    _x = -32767;
  }
  return static_cast<short>(_x);
}
/// @brief 指定されたIMU/AHRSタイプに基づいて, 計測したAHRSデータを読み込む.
/// @param a_type 使用するセンサのタイプを示す列挙（MPU6050, MPU9250, BNO055）.
/// @param a_ahrs_result AHRSから読み取った結果を格納した配列.
/// @return データの書き込みが成功した場合はtrue, それ以外の場合はfalseを返す.
bool meriput90_ahrs(MeridimUnion &a_meridim, float a_ahrs_result[], int a_type, bool &imuahrs_available) {
  if (a_type == BNO055_AHRS) {
    imuahrs_available = false;
    a_meridim.sval[2] = mrd_float2HfShort(a_ahrs_result[0]);   // IMU/AHRS_acc_x
    a_meridim.sval[3] = mrd_float2HfShort(a_ahrs_result[1]);   // IMU/AHRS_acc_y
    a_meridim.sval[4] = mrd_float2HfShort(a_ahrs_result[2]);   // IMU/AHRS_acc_z
    a_meridim.sval[5] = mrd_float2HfShort(a_ahrs_result[3]);   // IMU/AHRS_gyro_x
    a_meridim.sval[6] = mrd_float2HfShort(a_ahrs_result[4]);   // IMU/AHRS_gyro_y
    a_meridim.sval[7] = mrd_float2HfShort(a_ahrs_result[5]);   // IMU/AHRS_gyro_z
    a_meridim.sval[8] = mrd_float2HfShort(a_ahrs_result[6]);   // IMU/AHRS_mag_x
    a_meridim.sval[9] = mrd_float2HfShort(a_ahrs_result[7]);   // IMU/AHRS_mag_y
    a_meridim.sval[10] = mrd_float2HfShort(a_ahrs_result[8]);  // IMU/AHRS_mag_z
    a_meridim.sval[11] = mrd_float2HfShort(a_ahrs_result[15]); // temperature
    a_meridim.sval[12] = mrd_float2HfShort(a_ahrs_result[12]); // DMP_ROLL推定値
    a_meridim.sval[13] = mrd_float2HfShort(a_ahrs_result[13]); // DMP_PITCH推定値
    a_meridim.sval[14] = mrd_float2HfShort(a_ahrs_result[14]); // DMP_YAW推定値
    imuahrs_available = true;
    return true;
  }
  return false;
}

#endif // __MERIDIAN_MODULE_MRD_WIRE0_HPP__
