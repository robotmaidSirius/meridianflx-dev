# ビルド前に実行されるスクリプト
# Reference:
#   https://docs.platformio.org/en/latest/manifests/library-json/fields/build/extrascript.html

Import("env")

import mrd_setup as mrd_setup

if env.IsIntegrationDump():
   # stop the current script execution
   Return()
print("=====================================================")

# Update submodules
project_name = env["PIOENV"]
print(f"# Project Name: {project_name}")
mrd_setup.create_keyfile(f"{project_name}")
mrd_setup.create_parameter(f"{project_name}")

print("=====================================================")
