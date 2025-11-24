# Meridianの初期設定を行うスクリプト

import subprocess
import os
import datetime
import json
import pathlib

def update_submodule():
    ########################################
    # サブモジュールの初期化と更新
    ########################################
    result = subprocess.run(["git", "submodule", "update", "--init", "--recursive"], capture_output=True, text=True)
    print("# [Info] git submodule update --init --recursive")
    if result.stdout:
        print(result.stdout)

def create_board_ip_txt(root_dir="../"):
    ########################################
    # Meridian_consoleのboard_ip.txtの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.abspath(os.path.join(root_dir, "tools", "Meridian_console"))
    file_path = os.path.abspath(os.path.join(folder_path, "board_ip.txt"))

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)

    # If the file already exists, do not overwrite it
    if not os.path.exists(file_path):
        with open(file_path, "w") as file:
            file.write('UDP_SEND_IP_DEF="192.168.xxx.xxx"')
        print(f'# [Info] "board_ip.txt" has been created at {file_path}')
    else:
        print(f'# [Info] "board_ip.txt" already exists at {file_path}')

def create_keyfile(sub_dir, root_dir="../../"):
    ########################################
    # meridian_network_keys.hppの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.abspath(os.path.join(root_dir, "src", sub_dir))
    file_path = os.path.abspath(os.path.join(folder_path, "meridian_network_keys.hpp"))

    if os.path.exists(file_path):
        print(f'# [Info] Skipping creation : "meridian_network_keys.hpp" already exists at {file_path}')
        return

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)
    WIFI_AP_SSID = os.getenv('WIFI_AP_SSID', 'xxxxxxxx')
    WIFI_AP_PASS = os.getenv('WIFI_AP_PASS', 'xxxxxxxx')
    WIFI_SEND_IP = os.getenv('WIFI_SEND_IP', '192.168.1.xxx')
    UDP_SEND_PORT = os.getenv('UDP_SEND_PORT', '40009')
    UDP_RECEIVED_PORT = os.getenv('UDP_RECEIVED_PORT', '40009')

    MODE_FIXED_IP = os.getenv('MODE_FIXED_IP', '0')
    FIXED_IP_ADDR = os.getenv('FIXED_IP_ADDR', '192.168.1.xxx')
    FIXED_IP_GATEWAY = os.getenv('FIXED_IP_GATEWAY', '192.168.1.xxx')
    FIXED_IP_SUBNET = os.getenv('FIXED_IP_SUBNET', '255.255.255.0')
    MERIDIAN_BOARD_NAME = os.getenv('MERIDIAN_BOARD_NAME', 'MeridianBoard')

    # If the file already exists, do not overwrite it
    with open(file_path, "w") as file:
        file.write('#ifndef __MERIDIAN_NETWORK_KEYS_HPP__\n')
        file.write('#define __MERIDIAN_NETWORK_KEYS_HPP__\n\n')

        file.write('// Wifiアクセスポイントの設定\n')
        file.write('#define WIFI_AP_SSID        "' + WIFI_AP_SSID           + '"   ///! アクセスポイントのWIFI_AP_SSID\n')
        file.write('#define WIFI_AP_PASS        "' + WIFI_AP_PASS           + '"   ///! アクセスポイントのパスワード\n')
        file.write('#define WIFI_SEND_IP        "' + WIFI_SEND_IP           + '"   ///! 送り先のPCのIPアドレス（PCのIPアドレスを調べておく）\n')
        file.write('#define UDP_SEND_PORT       (' + UDP_SEND_PORT          + ')   ///! 送り先のポート番号\n')
        file.write('#define UDP_RECEIVED_PORT   (' + UDP_RECEIVED_PORT      + ')   ///! このESP32のポート番号\n')
        file.write('#define MERIDIAN_BOARD_NAME "' + MERIDIAN_BOARD_NAME    + '"   ///! このボードのホスト名\n')
        file.write('\n')

        file.write('// ESP32のIPアドレスを固定する場合は下記の4項目を設定\n')
        file.write('#define MODE_FIXED_IP    (' + MODE_FIXED_IP     + ')    ///! IPアドレスを固定するか（0:NO, 1:YES）\n')
        file.write('#define FIXED_IP_ADDR    "' + FIXED_IP_ADDR     + '"    ///! ESP32のIPアドレスを固定時のESPのIPアドレス\n')
        file.write('#define FIXED_IP_GATEWAY "' + FIXED_IP_GATEWAY  + '"    ///! ESP32のIPアドレスを固定時のルーターのゲートウェイ\n')
        file.write('#define FIXED_IP_SUBNET  "' + FIXED_IP_SUBNET   + '"    ///! ESP32のIPアドレスを固定時のサブネット\n')

        file.write('\n#endif // __MERIDIAN_NETWORK_KEYS_HPP__\n')
    print(f'# [Info] "meridian_network_keys.hpp" has been created at {file_path}')

def create_parameter(project_name, root_dir=""):
    ########################################
    # meridian_parameter.hppの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.join(root_dir, "src", project_name)
    file_path = os.path.abspath(os.path.join(folder_path, "meridian_parameter.hpp"))
    library_json_path = os.path.abspath(os.path.join(root_dir, "library.json"))

    if os.path.exists(file_path):
        print(f'# [Info] Skipping creation : "meridian_parameter.hpp" already exists at {file_path}')
        return

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)
    # Check if the file already exists, and if so, do not overwrite it
    if os.path.exists(file_path):
        os.remove(file_path)

    BUILD_TIME = int(datetime.datetime.timestamp(datetime.datetime.now()))
    BUILD_VERSION = "1.0.0"
    BUILD_SYSTEM_NAME = "MeridianFlx"
    if os.path.exists(library_json_path):
        with open(library_json_path) as f:
            j = json.load(f)
            if "version" in j:
                BUILD_VERSION = j["version"]
            if "name" in j:
                BUILD_SYSTEM_NAME = j["name"]
    # If the file already exists, do not overwrite it
    with open(file_path, "w", encoding='utf-8') as file:
        file.write('#ifndef __MERIDIAN_PARAMETER_HPP__\n')
        file.write('#define __MERIDIAN_PARAMETER_HPP__\n\n')

        file.write('// ビルド情報\n')
        file.write('#define MERIDIAN_SYSTEM_NAME    "' + str(BUILD_SYSTEM_NAME) + '"   /*! システム名*/\n')
        file.write('#define MERIDIAN_BOARD_NAME     "' + str(project_name)       + '"   /*! ボード名*/\n')
        file.write('#define MERIDIAN_BUILD_TIME     (' + str(BUILD_TIME)    + ')   /*! ビルド時刻*/\n')
        file.write('#define MERIDIAN_BUILD_VERSION  "' + BUILD_VERSION      + '"   /*! ビルドバージョン*/\n')

        file.write('// ログレベル\n')
        file.write('#define MERIDIAN_DEFAULT_LOG_LEVEL        OUTPUT_LOG_LEVEL::LEVEL_ALL\n')

        file.write('\n#endif // __MERIDIAN_PARAMETER_HPP__\n')
    print(f'# [Info] "meridian_parameter.hpp" has been created at {file_path}.')

if __name__ == '__main__':
    script_dir = pathlib.Path(__file__).parent
    script_dir = os.path.join(script_dir,"../../")
    project_list = [
        "Meridian_LITE_M5StackAtomLite",
        "Meridian_LITE_M5StackAtomS3",
        "Meridian_LITE_ESP32",
        "Meridian_TWIN_ESP32",
        "Meridian_TWIN_Tsy40"
    ]

    update_submodule()
    create_board_ip_txt(script_dir)
    for project_name in project_list:
        create_keyfile(project_name, script_dir)
        create_parameter(project_name, script_dir)
