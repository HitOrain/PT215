
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)

./ndk-build NDK_PROJECT_PATH=$SHELL_FOLDER/ndt
rm -rf ./ndt/obj

cp ./ndt/libs/arm64-v8a/*.so ./ndt/jni/prebuild/

bash release.sh
