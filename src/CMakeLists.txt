find_package(benchmark REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(blf SHARED test.cpp ${SOURCE})

target_link_libraries(blf PRIVATE
        ZLIB::ZLIB
        utility
)
target_include_directories(blf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
if (WIN32)
    target_compile_definitions(blf PRIVATE CANCONTROL_EXPORTS)
endif ()

if (WIN32)
    # 导出CANCONTROL_API 到lib中
    target_compile_definitions(blf PRIVATE TWBLF_EXPORTS)
    # 链接Windows所需标准库
    target_link_libraries(blf PRIVATE ws2_32 iphlpapi winmm)
    # 指定Windows版本
    target_compile_definitions(blf PRIVATE _WIN32_WINNT=0x0A00)
endif ()

add_subdirectory(utility)
add_dependencies(blf utility)