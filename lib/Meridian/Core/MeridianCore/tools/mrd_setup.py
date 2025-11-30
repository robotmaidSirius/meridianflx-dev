# Meridianの初期設定を行うスクリプト
import os
import pathlib

def create_settings_file(root_dir=""):
    ########################################
    # meridian_parameter.hppの作成
    ########################################
    # 「tools/Meridian_console」フォルダのパスを定義
    folder_path = os.path.join(root_dir, "include")
    file_path = os.path.abspath(os.path.join(folder_path, "meridian_core_settings.hpp"))

    if os.path.exists(file_path):
        print(f'## [Info] Already exists: {file_path}')
        return

    # フォルダが存在しない場合は作成
    os.makedirs(folder_path, exist_ok=True)
    # Check if the file already exists, and if so, do not overwrite it
    if os.path.exists(file_path):
        print(f'## [Info] Already exists: {file_path}')
        return

    # If the file already exists, do not overwrite it
    with open(file_path, "w", encoding='utf-8') as file:
        file.write('#ifndef __MERIDIAN_CORE_SETTINGS_HPP__\n')
        file.write('#define __MERIDIAN_CORE_SETTINGS_HPP__\n\n')

        file.write('#ifndef IMrdApp_DEFAULT_INTERVAL_US\n')
        file.write('/// @brief デフォルト実行インターバル（マイクロ秒単位）\n')
        file.write('#define IMrdApp_DEFAULT_INTERVAL_US (10 * 1000)\n')
        file.write('#endif\n')
        file.write('#ifndef IMrdDriver_DEFAULT_INTERVAL_US\n')
        file.write('/// @brief デフォルト実行インターバル（マイクロ秒単位）\n')
        file.write('#define IMrdDriver_DEFAULT_INTERVAL_US (10 * 1000)\n')
        file.write('#endif\n')

        file.write('\n#endif // __MERIDIAN_CORE_SETTINGS_HPP__\n')

    print(f'## [Mess] Created at {file_path}')

if __name__ == '__main__':
    script_dir = pathlib.Path(__file__).parent
    script_dir = os.path.join(script_dir,"../")

    create_settings_file(script_dir)
