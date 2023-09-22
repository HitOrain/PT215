adb root
adb remount

adb push ndt/libs/arm64-v8a/libndt.so /vendor/lib64 
adb push ndt/libs/arm64-v8a/ndt_algorithm /vendor/bin

REM adb reboot
REM adb wait-for-device

REM adb root 
REM adb remount

REM pause()