rm -rf ~/Public/NDK_vivo/android-ndk-r11c/ndt/jni/main
rm -rf ~/Public/NDK_vivo/android-ndk-r11c/ndt/jni/prebuild
rm -rf ~/Public/NDK_vivo/android-ndk-r11c/ndt/libs
rm -rf ~/Music/NDK/android-ndk-r11c/release/*

cp ndt/jni/main/ ~/Public/NDK_vivo/android-ndk-r11c/ndt/jni/ -rf
cp ndt/jni/prebuild/ ~/Public/NDK_vivo/android-ndk-r11c/ndt/jni/ -rf
cp ndt/libs/  ~/Public/NDK_vivo/android-ndk-r11c/ndt/ -rf

cp ~/Public/NDK_vivo/android-ndk-r11c/ndt ./release/et3089x_pt215_NDT_Algo_So_For_vivo_v42.15 -rf
