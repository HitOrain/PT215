adb root
adb remount

adb push libndt.so /vendor/lib64 
adb push ndt_algorithm /vendor/bin

adb reboot

pause()