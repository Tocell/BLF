
file(GLOB_RECURSE UTILITY_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(utility STATIC ${UTILITY_SOURCE})
target_include_directories(utility PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/log
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool
)
target_link_libraries(utility PUBLIC log)

 if (WIN32)
     target_compile_definitions(utility PRIVATE CANCONTROL_EXPORTS)
 endif ()

add_subdirectory(log)
add_dependencies(utility log)
