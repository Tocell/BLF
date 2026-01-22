add_executable(write_blf write_blf.cpp)
target_link_libraries(write_blf blf)

add_executable(write_asc write_asc.cpp)
target_link_libraries(write_asc blf)

add_executable(parser_blf parser_blf.cpp)
target_link_libraries(parser_blf blf)

add_executable(parser_asc parser_asc.cpp)
target_link_libraries(parser_asc blf)