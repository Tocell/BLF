find_package(benchmark REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB SOURCE CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/api/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/io/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/registry/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/can_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/can_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/lin_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/lin_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/flexray_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/flexray_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/ethernet_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/ethernet_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/can_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/can_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/lin_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/lin_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/flexray_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/flexray_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/ethernet_writer/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/ethernet_reader/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/can/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/lin/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/flexray/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/ethernet/*.cpp
)

add_library(blf SHARED ${SOURCE})

target_link_libraries(blf PRIVATE
        ZLIB::ZLIB
)
target_include_directories(blf
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/can
        ${CMAKE_CURRENT_SOURCE_DIR}/include/lin
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flexray
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ethernet

        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/core/api
        ${CMAKE_CURRENT_SOURCE_DIR}/core/io
        ${CMAKE_CURRENT_SOURCE_DIR}/core/registry
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/can_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/can_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/lin_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/lin_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/flexray_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/flexray_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/ethernet_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/blf/ethernet_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/can_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/can_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/lin_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/lin_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/flexray_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/flexray_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/ethernet_writer
        ${CMAKE_CURRENT_SOURCE_DIR}/formats/asc/ethernet_reader
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/can
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/lin
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/flexray
        ${CMAKE_CURRENT_SOURCE_DIR}/messages/ethernet
)

if (WIN32)
    # 导出 BLF_EXPORTS 到lib中
    target_compile_definitions(blf PRIVATE GWLOGGER_EXPORTS)
    # 链接Windows所需标准库
    target_link_libraries(blf PRIVATE ws2_32 iphlpapi winmm)
    # 指定Windows版本
    target_compile_definitions(blf PRIVATE _WIN32_WINNT=0x0A00)
endif ()

add_subdirectory(examples)
