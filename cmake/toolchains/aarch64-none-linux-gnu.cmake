####################################
# arm-none-eabi-gcc Base Toolchain #
####################################
#
# To include this file as a base toolchain file,
# include it at the bottom of the derived toolchain file.


#################
# System Config #
#################

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm64)
# Represents the name of the specific processor type, e.g. Cortex-A9
if(NOT CPU_NAME)
    set(CPU_NAME generic)
endif()

####################
# Toolchain Config #
####################
set(TOOLCHAIN_ROOT          "/home/twh/Downloads/host/bin/")
set(TOOLCHAIN_PREFIX        "aarch64-linux-")

set(CMAKE_C_COMPILER        ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER      ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER      ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER            ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}g++)
set(CMAKE_AR                ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}ar)
set(CMAKE_OBJCOPY           ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP           ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_RANLIB            ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}ranlib)
set(CMAKE_STRIP             ${TOOLCHAIN_ROOT}${TOOLCHAIN_PREFIX}strip)

# If set to ONLY, then only the roots in CMAKE_FIND_ROOT_PATH (i.e., the host machine)
# will be searched. If set to NEVER, then the roots in CMAKE_FIND_ROOT_PATH will
# be ignored and only the build machine root will be used.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

################
# Common Flags #
################
# See the CMake Manual for CMAKE_<LANG>_FLAGS_INIT:
#	https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_FLAGS_INIT.html

set(CMAKE_C_FLAGS_INIT
        ""
        CACHE
        INTERNAL "Default C compiler flags.")
set(CMAKE_CXX_FLAGS_INIT
        ""
        CACHE
        INTERNAL "Default C++ compiler flags.")
set(CMAKE_ASM_FLAGS_INIT
        "-x assembler-with-cpp"
        CACHE
        INTERNAL "Default ASM compiler flags.")
set(CMAKE_EXE_LINKER_FLAGS_INIT
        ""
        CACHE
        INTERNAL "Default exe linker flags.")

include(${CMAKE_CURRENT_LIST_DIR}/OverrideBuildTypeSettings.cmake)
