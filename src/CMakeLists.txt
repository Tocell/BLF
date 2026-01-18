find_package(benchmark REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/blf/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/blf/can_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/blf/can_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/asc/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/asc/can_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/asc/can_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/object/can/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/registry/*.cpp
)

add_library(blf SHARED ${SOURCE})

target_link_libraries(blf PRIVATE
        ZLIB::ZLIB
        utility
)
target_include_directories(blf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/blf
        ${CMAKE_CURRENT_SOURCE_DIR}/blf/can_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/blf/can_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/api
        ${CMAKE_CURRENT_SOURCE_DIR}/io
        ${CMAKE_CURRENT_SOURCE_DIR}/asc
        ${CMAKE_CURRENT_SOURCE_DIR}/asc/can_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/asc/can_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/object/can
        ${CMAKE_CURRENT_SOURCE_DIR}/registry
)

if (WIN32)
    # 导出 BLF_EXPORTS 到lib中
    target_compile_definitions(blf PRIVATE BLF_EXPORTS)
    # 链接Windows所需标准库
    target_link_libraries(blf PRIVATE ws2_32 iphlpapi winmm)
    # 指定Windows版本
    target_compile_definitions(blf PRIVATE _WIN32_WINNT=0x0A00)
endif ()

add_subdirectory(utility)
add_dependencies(blf utility)