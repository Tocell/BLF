find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(benchmark REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE SOURCE
        ${CMAKE_SOURCE_DIR}/src/file/*.cpp
        ${CMAKE_SOURCE_DIR}/src/object/*.cpp
        ${CMAKE_SOURCE_DIR}/src/object/can_message/*.cpp
        ${CMAKE_SOURCE_DIR}/src/object/lin_message/*.cpp
        ${CMAKE_SOURCE_DIR}/src/object/flexray_message/*.cpp
        ${CMAKE_SOURCE_DIR}/src/object/object_header/*.cpp
)

add_library(blf SHARED ${SOURCE})
target_include_directories(blf PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/file
        ${CMAKE_CURRENT_SOURCE_DIR}/object
        ${CMAKE_CURRENT_SOURCE_DIR}/object/can_message
        ${CMAKE_CURRENT_SOURCE_DIR}/object/lin_message
        ${CMAKE_CURRENT_SOURCE_DIR}/object/flexray_message
        ${CMAKE_CURRENT_SOURCE_DIR}/object/object_header
)
target_link_libraries(blf PRIVATE
        spdlog::spdlog
        ZLIB::ZLIB
)

add_executable(demo demo/main.cpp)
target_link_libraries(demo PUBLIC blf)

add_executable(parse demo/parse.cpp)
target_link_libraries(parse PUBLIC blf)

add_executable(benchmark_test demo/benchmark.cpp)
target_link_libraries(benchmark_test PUBLIC blf benchmark::benchmark)

if (WIN32)
    # 导出CANCONTROL_API 到lib中
    target_compile_definitions(blf PRIVATE TWBLF_EXPORTS)
    # 链接Windows所需标准库
    target_link_libraries(blf PRIVATE ws2_32 iphlpapi winmm)
    # 指定Windows版本
    target_compile_definitions(blf PRIVATE _WIN32_WINNT=0x0A00)
endif ()
