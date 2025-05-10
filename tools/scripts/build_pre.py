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

    WIFI_AP_SSID = os.getenv('WIFI_AP_SSID', 'xxxxxxxx')
    WIFI_AP_PASS = os.getenv('WIFI_AP_PASS', 'xxxxxxxx')
    WIFI_SEND_IP = os.getenv('WIFI_SEND_IP', '192.168.1.xxx')
    UDP_SEND_PORT = os.getenv('UDP_SEND_PORT', '22222')
    UDP_RECEIVED_PORT = os.getenv('UDP_RESV_PORT', '22224')

    MODE_FIXED_IP = os.getenv('MODE_FIXED_IP', '0')
    FIXED_IP_ADDR = os.getenv('FIXED_IP_ADDR', '192.168.1.xxx')
    FIXED_IP_GATEWAY = os.getenv('FIXED_IP_GATEWAY', '192.168.1.xxx')
    FIXED_IP_SUBNET = os.getenv('FIXED_IP_SUBNET', '255.255.255.0')

    # If the file already exists, do not overwrite it
    with open(file_path, "w", encoding='utf-8') as file:
        file.write('#ifndef __MERIDIAN_KEYS_H__\n')
        file.write('#define __MERIDIAN_KEYS_H__\n\n')

        file.write('// Wifiアクセスポイントの設定\n')
        file.write('#define WIFI_AP_SSID  "' + WIFI_AP_SSID      + '"   ///! アクセスポイントのWIFI_AP_SSID\n')
        file.write('#define WIFI_AP_PASS  "' + WIFI_AP_PASS      + '"   ///! アクセスポイントのパスワード\n')
        file.write('#define WIFI_SEND_IP  "' + WIFI_SEND_IP      + '"   ///! 送り先のPCのIPアドレス（PCのIPアドレスを調べておく）\n')
        file.write('#define UDP_SEND_PORT (' + UDP_SEND_PORT     + ')   ///! 送り先のポート番号\n')
        file.write('#define UDP_RESV_PORT (' + UDP_RECEIVED_PORT + ')   ///! このESP32のポート番号\n\n')

        file.write('// ESP32のIPアドレスを固定する場合は下記の4項目を設定\n')
        file.write('#define MODE_FIXED_IP    (' + MODE_FIXED_IP     + ')    ///! IPアドレスを固定するか（0:NO, 1:YES）\n')
        file.write('#define FIXED_IP_ADDR    "' + FIXED_IP_ADDR     + '"    ///! ESP32のIPアドレスを固定時のESPのIPアドレス\n')
        file.write('#define FIXED_IP_GATEWAY "' + FIXED_IP_GATEWAY  + '"    ///! ESP32のIPアドレスを固定時のルーターのゲートウェイ\n')
        file.write('#define FIXED_IP_SUBNET  "' + FIXED_IP_SUBNET   + '"    ///! ESP32のIPアドレスを固定時のサブネット\n\n')
        file.write('#endif // __MERIDIAN_KEYS_H__\n')
    print(f'# [Info] "keys.h" has been created at {file_path}')

if env.IsIntegrationDump():
   # stop the current script execution
   Return()
print("=====================================================")

# Update submodules
project_name = env["PIOENV"]
print(f"# Project Name: {project_name}")
crate_keyfile(f"{project_name}")

print("=====================================================")
