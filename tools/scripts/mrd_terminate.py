import subprocess
import os
import pathlib

#print("git update-index --no-skip-worktree ../Meridian_LITE_for_ESP32/src/keys.h")
#result = subprocess.run(["git", "update-index", "--no-skip-worktree", "../Meridian_LITE_for_ESP32/src/keys.h"], capture_output=True, text=True)
#print(result.stdout)

# 削除対象のファイル
target_files = [
    "meridian_network_keys.hpp",
    "meridian_parameter.hpp"
]

# srcフォルダのパスを取得（スクリプトから相対パス）
script_dir = pathlib.Path(__file__).parent
src_dir = script_dir.parent.parent / "src"

print(f"Searching in: {src_dir}")

# srcフォルダ内を再帰的に検索して削除
for target_file in target_files:
    for file_path in src_dir.rglob(target_file):
        if file_path.exists():
            try:
                file_path.unlink()
                print(f"# Deleted: {file_path}")
            except Exception as e:
                print(f"# Error deleting {file_path}: {e}")
        else:
            print(f"# Not found: {target_file}")

print("Process completed.")
