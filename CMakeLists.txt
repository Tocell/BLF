cmake_minimum_required(VERSION 3.20)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    include(cmake/vcpkg/vcpkg.cmake)
endif()
project(blf_logger VERSION 1.0.1)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/output"
        CACHE PATH "Default installation path" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
option(BLF_BUILD_EXAMPLES "Build example programs" ON)
option(BLF_BUILD_BENCHMARKS "Build benchmark program" OFF)

if (MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

# 引入标准目录变量 (CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, etc.)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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

# 生成最终的 blfConfig.cmake 配置文件
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/blfConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/blfConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/blf"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/blfConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion
)

install(EXPORT blfTargets
        FILE blfTargets.cmake
        NAMESPACE blf::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/blf"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/blfConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/blfConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/blf"
)
