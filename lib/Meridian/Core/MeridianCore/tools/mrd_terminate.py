import pathlib

# 削除対象のファイル
target_files = [
    "meridian_core_settings.hpp"
]

# includeフォルダのパスを取得（スクリプトから相対パス）
script_dir = pathlib.Path(__file__).parent
include_dir = script_dir.parent.parent

print(f"Searching in: {include_dir}")
# includeフォルダ内を再帰的に検索して削除
for target_file in target_files:
    for file_path in include_dir.rglob(target_file):
        if file_path.exists():
            try:
                file_path.unlink()
                print(f"# Deleted: {file_path}")
            except Exception as e:
                print(f"# Error deleting {file_path}: {e}")
        else:
            print(f"# Not found: {target_file}")

print("Completed.")
