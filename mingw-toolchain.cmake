# CMake Toolchain file for cross-compiling to Windows using local LLVM-MinGW
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(TOOLCHAIN_PATH "/home/lali/Desktop/yabba/llvm-mingw-20260505-ucrt-ubuntu-22.04-x86_64")

# Specify the cross compiler
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/bin/x86_64-w64-mingw32-clang")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/bin/x86_64-w64-mingw32-clang++")
set(CMAKE_RC_COMPILER "${TOOLCHAIN_PATH}/bin/x86_64-w64-mingw32-windres")

# Where is the target environment located
set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_PATH}/x86_64-w64-mingw32")

# Adjust the default behavior of the FIND_XXX() commands:
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Required for JUCE with LLVM-MinGW
add_definitions(-D__CRT_UUID_DECL)
add_definitions(-D_WIN64)
add_definitions(-DJUCE_64BIT=1)
add_definitions(-DJUCE_DIRECT2D=0)
add_definitions(-DJUCE_USE_DIRECTWRITE=0)
add_compile_options(-m64)
