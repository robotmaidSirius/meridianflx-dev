# Meridianの初期設定を行うスクリプト

import subprocess
import os
import datetime
import json
import pathlib
import socket

def update_submodule():
    ########################################
    # サブモジュールの初期化と更新
    ########################################
    result = subprocess.run(["git", "submodule", "update", "--init", "--recursive"], capture_output=True, text=True)
    print("# [Info] git submodule update --init --recursive")
    if result.stdout:
        print(result.stdout)

def create_board_ip_txt(root_dir="."):
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
        # HostNameで検索し、そのIPアドレスを取得する
        SEARCH_HOSTNAME = os.getenv('MERIDIAN_BOARD_NAME', 'MeridianBoard')
        try:
            ip_address = socket.gethostbyname(SEARCH_HOSTNAME)
        except socket.gaierror:
            ip_address = os.getenv('MERIDIAN_BOARD_FIXED_IP_ADDR', 'MeridianBoard')
            print(f'## [Warning] Could not resolve "{SEARCH_HOSTNAME}"')
        with open(file_path, "w", encoding='utf-8') as file:
            file.write(f'UDP_SEND_IP_DEF="{ip_address}"\n')
        print(f'## [Mess] Created at {file_path}')
    else:
        print(f'## [Info] Already exists: {file_path}')

def create_keyfile(sub_dir, root_dir="."):
    ########################################
    # meridian_network_keys.hppの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.abspath(os.path.join(root_dir, "src", sub_dir))
    file_path = os.path.abspath(os.path.join(folder_path, "meridian_network_keys.hpp"))

    if os.path.exists(file_path):
        print(f'## [Info] Already exists: {file_path}')
        return

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)
    HOST_NAME = os.getenv('MERIDIAN_HOST_NAME', 'MeridianBoard')

    WIFI_AP_SSID = os.getenv('MERIDIAN_BOARD_WIFI_SSID', 'xxxxxxxx')
    WIFI_AP_PASS = os.getenv('MERIDIAN_BOARD_WIFI_PASS', 'xxxxxxxx')
    UDP_SEND_IP = os.getenv('MERIDIAN_UDP_TO_HOST_IP', '192.168.1.xxx')
    UDP_SEND_PORT = os.getenv('MERIDIAN_UDP_TO_HOST_PORT', '40009')
    UDP_RECEIVED_PORT = os.getenv('MERIDIAN_UDP_FROM_HOST_PORT', '40009')

    MODE_FIXED_IP = os.getenv('MERIDIAN_BOARD_FIXED_IP_FLAG', '0')
    FIXED_IP_ADDR = os.getenv('MERIDIAN_BOARD_FIXED_IP_ADDR', '192.168.1.100')
    FIXED_IP_GATEWAY = os.getenv('MERIDIAN_BOARD_FIXED_IP_GATEWAY', '192.168.1.1')
    FIXED_IP_SUBNET = os.getenv('MERIDIAN_BOARD_FIXED_IP_SUBNET', '255.255.255.0')

    if UDP_SEND_IP == "192.168.1.xxx":
        SEARCH_HOSTNAME = "localhost"
        try:
            UDP_SEND_IP = socket.gethostbyname(SEARCH_HOSTNAME)
            print(f'## [Info] Resolved "{SEARCH_HOSTNAME}" to IP address: {UDP_SEND_IP}')
        except socket.gaierror:
            UDP_SEND_IP =  "192.168.1.xxx"
            print(f'## [Warning] Could not resolve "{SEARCH_HOSTNAME}", using default IP: {UDP_SEND_IP}')

    # If the file already exists, do not overwrite it
    with open(file_path, "w", encoding='utf-8') as file:
        file.write('#ifndef __MERIDIAN_NETWORK_KEYS_HPP__\n')
        file.write('#define __MERIDIAN_NETWORK_KEYS_HPP__\n')

        file.write('\n/* Wifiアクセスポイントの設定 */\n')
        file.write('#define NETWORK_WIFI_SSID      "' + WIFI_AP_SSID       + '"   /*! アクセスポイントのSSID */\n')
        file.write('#define NETWORK_WIFI_PASS      "' + WIFI_AP_PASS       + '"   /*! アクセスポイントのパスワード */\n')
        file.write('#define NETWORK_SEND_IP        "' + UDP_SEND_IP        + '"   /*! 送り先のPCのIPアドレス (PCのIPアドレスを調べておく) */\n')
        file.write('#define NETWORK_SEND_PORT      (' + UDP_SEND_PORT      + ')   /*! 送り先のポート番号 */\n')
        file.write('#define NETWORK_RECEIVED_PORT  (' + UDP_RECEIVED_PORT  + ')   /*! このESP32のポート番号 */\n')
        file.write('#define NETWORK_HOST_NAME      "' + HOST_NAME          + '"   /*! このボードのホスト名 */\n')

        file.write('\n/* ESP32のIPアドレスを固定する場合は下記の4項目を設定 */\n')
        file.write('#define NETWORK_MODE_FIXED_IP  (' + MODE_FIXED_IP     + ')    /*! IPアドレスを固定するか (0:NO, 1:YES) */\n')
        file.write('#define NETWORK_FIXED_IP       "' + FIXED_IP_ADDR     + '"    /*! ESP32のIPアドレスを固定時のESPのIPアドレス */\n')
        file.write('#define NETWORK_FIXED_GATEWAY  "' + FIXED_IP_GATEWAY  + '"    /*! ESP32のIPアドレスを固定時のルーターのゲートウェイ */\n')
        file.write('#define NETWORK_FIXED_SUBNET   "' + FIXED_IP_SUBNET   + '"    /*! ESP32のIPアドレスを固定時のサブネット */\n')

        file.write('\n#endif /* __MERIDIAN_NETWORK_KEYS_HPP__ */\n')
    print(f'## [Mess] Created at {file_path}')

def create_parameter(project_name, root_dir=""):
    ########################################
    # meridian_parameter.hppの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.join(root_dir, "src", project_name)
    file_path = os.path.abspath(os.path.join(folder_path, "meridian_parameter.hpp"))
    library_json_path = os.path.abspath(os.path.join(root_dir, "library.json"))

    if os.path.exists(file_path):
        print(f'## [Info] Already exists: {file_path}')
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
        with open(library_json_path, encoding='utf-8') as f:
            j = json.load(f)
            if "version" in j:
                BUILD_VERSION = j["version"]
            if "name" in j:
                BUILD_SYSTEM_NAME = j["name"]
    # If the file already exists, do not overwrite it
    with open(file_path, "w", encoding='utf-8') as file:
        file.write('#ifndef __MERIDIAN_PARAMETER_HPP__\n')
        file.write('#define __MERIDIAN_PARAMETER_HPP__\n')

        file.write('\n/* ビルド情報 */\n')
        file.write('#define MERIDIAN_SYSTEM_NAME    "' + str(BUILD_SYSTEM_NAME) + '"   /*! システム名 */\n')
        file.write('#define MERIDIAN_BOARD_NAME     "' + str(project_name)      + '"   /*! ボード名 */\n')
        file.write('#define MERIDIAN_BUILD_VERSION  "' + str(BUILD_VERSION)     + '"   /*! ビルドバージョン */\n')
        file.write('#ifndef MERIDIAN_BUILD_TIME\n')
        file.write('#define MERIDIAN_BUILD_TIME     (' + str(BUILD_TIME)    + ')   /*! ビルド時刻 */\n')
        file.write('#endif\n')

        file.write('\n/* ログレベル */\n')
        file.write('#define MERIDIAN_DEFAULT_LOG_LEVEL  (OUTPUT_LOG_LEVEL::LEVEL_ALL)\n')

        file.write('\n#endif /* __MERIDIAN_PARAMETER_HPP__ */\n')
    print(f'## [Mess] Created at {file_path}')

if __name__ == '__main__':
    script_dir = pathlib.Path(__file__).parent
    script_dir = os.path.join(script_dir,"../../")
    project_list = [
        "Meridian_LITE_M5StackAtomLite",
        "Meridian_LITE_M5StackAtomS3",
        "Meridian_LITE_ESP32",
        "Meridian_TWIN_ESP32",
        "Meridian_TWIN_Teensy40"
    ]

    update_submodule()
    create_board_ip_txt(script_dir)
    for project_name in project_list:
        create_keyfile(project_name, script_dir)
        create_parameter(project_name, script_dir)
