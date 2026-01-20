cmake_minimum_required(VERSION 3.20)
include(cmake/conan/conan.cmake)
project(blf_logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

if (MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

# 引入标准目录变量 (CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, etc.)
include(GNUInstallDirs)

# --- 统一输出目录设置 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# 遍历每一个目录下的cmake均进行处理
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

add_subdirectory(src)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/output"
        CACHE PATH "Default installation path" FORCE)

