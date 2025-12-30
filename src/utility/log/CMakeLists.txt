find_package(spdlog REQUIRED)

add_library(log STATIC log.cpp)

target_link_libraries(log spdlog::spdlog)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    # 用于 Windows下 导致符号链接   或者在库头文件中导出
    target_compile_definitions(log PRIVATE MODULE_EXPORTS)

    set_target_properties(log PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif ()
