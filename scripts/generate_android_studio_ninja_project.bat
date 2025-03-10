:: Required Global Variables
:: NDK_CMAKE: Path to cmake from Android Studio package manager like "C:\Users\{USER}\AppData\Local\Android\Sdk\cmake\{CMAKE VERSION}"
:: NDK: Path to installed NDK like "C:\Users\{USER}\AppData\Local\Android\Sdk\ndk\{NDK VERSION}"

:: Arguments
:: 1. Path to source
:: 2. Cmake build type {Debug, Release, RelWithDebInfo}
:: 3. Target ABI {armeabi-v7a, arm64-v8a, x86, x86_64}

@echo off

set ABI=armeabi-v7a
IF NOT "%~3"=="" set ABI=%3

cd %NDK_CMAKE%/bin/

cmake^
    -DCMAKE_TOOLCHAIN_FILE="%NDK%/build/cmake/android.toolchain.cmake"^
    -DCMAKE_MAKE_PROGRAM="%NDK_CMAKE%/bin/ninja"^
    -DANDROID_ABI=%ABI%^
    -DCMAKE_ANDROID_ARCH_ABI=%ABI%^
    -DCMAKE_SYSTEM_NAME=Android^
    -DCMAKE_SYSTEM_VERSION=23^
    -DANDROID_PLATFORM=android-23^
    -DANDROID_STL=c++_static^
    -DCMAKE_BUILD_TYPE=%2^
    -S%1^
    -B%1/android_build^
    -GNinja

cd %1/build-android

call "%NDK_CMAKE%/bin/ninja"

pause
