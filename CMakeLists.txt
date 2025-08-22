cmake_minimum_required(VERSION 3.20)
include(cmake/conan/conan.cmake)
project(BLF)
#include(cmake/sanitizers/sanitizers.cmake)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_STANDARD 99)

if (MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

# 引入标准目录变量 (CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, etc.)
include(GNUInstallDirs)
# 强制设置安装路径为项目根目录下的 output
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/output"
        CACHE PATH "Default installation path" FORCE)

add_subdirectory(src)


### 安装规则 ###

# 1. 安装 src/object 目录及其子目录下的所有头文件
#    - DIRECTORY: 指定要安装的源目录
#    - DESTINATION: 指定安装到 <prefix>/include 目录下
#    - FILES_MATCHING: 只安装匹配模式的文件（这里是.h头文件）
install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/object/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
        PATTERN "abstract_file.h" EXCLUDE
        PATTERN "file_statistics.h" EXCLUDE
        PATTERN "log_container.h" EXCLUDE
)

# 2. 安装库文件 (blf)
#    - RUNTIME: .dll 文件 (Windows)
#    - LIBRARY: .so (Linux), .dll (Windows) 文件
#    - ARCHIVE: .lib (Windows), .a (Linux) 文件
install(TARGETS blf
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 3. 安装可执行文件 (demo, parse, benchmark_test)
install(TARGETS demo parse benchmark_test
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 4. 安装 demo 目录下的源文件到 <prefix>/demo 目录下
install (FILES
        src/demo/main.cpp
        src/demo/parse.cpp
        src/demo/benchmark.cpp
        DESTINATION "demo"
)
