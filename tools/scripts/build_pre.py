# Reference:
#   https://docs.platformio.org/en/latest/manifests/library-json/fields/build/extrascript.html
Import("env")

import subprocess
import os

def crate_keyfile(path):
    ########################################
    # keys.hの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.join("src", path)
    file_path = os.path.join(folder_path, "keys.h")

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)
    # Check if the file already exists, and if so, do not overwrite it
    if os.path.exists(file_path):
        print(f'# [Info] "keys.h" already exists at {file_path}. Skipping creation.')
        return

    # If the file already exists, do not overwrite it
    with open(file_path, "w") as file:
        file.write('#ifndef __MERIDIAN_KEYS_H__\n')
        file.write('#define __MERIDIAN_KEYS_H__\n\n')

        file.write('// Wifiアクセスポイントの設定\n')
        file.write('#define WIFI_AP_SSID  "xxxxxxxx"     // アクセスポイントのWIFI_AP_SSID\n')
        file.write('#define WIFI_AP_PASS  "xxxxxxxx"     // アクセスポイントのパスワード\n')
        file.write('#define WIFI_SEND_IP  "192.168.1.xx" // 送り先のPCのIPアドレス（PCのIPアドレスを調べておく）\n')
        file.write('#define UDP_SEND_PORT 22222          // 送り先のポート番号\n')
        file.write('#define UDP_RESV_PORT 22224          // このESP32のポート番号\n\n')

        file.write('// ESP32のIPアドレスを固定する場合は下記の4項目を設定\n')
        file.write('#define MODE_FIXED_IP    0               // IPアドレスを固定するか（0:NO, 1:YES）\n')
        file.write('#define FIXED_IP_ADDR    "192.168.1.xx"  // ESP32のIPアドレスを固定時のESPのIPアドレス\n')
        file.write('#define FIXED_IP_GATEWAY "192.168.1.xx"  // ESP32のIPアドレスを固定時のルーターのゲートウェイ\n')
        file.write('#define FIXED_IP_SUBNET  "255.255.255.0" // ESP32のIPアドレスを固定時のサブネット\n\n')
        file.write('#endif // __MERIDIAN_KEYS_H__\n')
    print(f'# [Info] "keys.h" has been created at {file_path}')

if env.IsIntegrationDump():
   # stop the current script execution
   Return()
print("=====================================================")

# Update submodules
crate_keyfile("Meridian_LITE_ESP32")
crate_keyfile("Meridian_TWIN_ESP32")
crate_keyfile("Meridian_TWIN_Tsy40")

print("=====================================================")
