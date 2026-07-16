add_executable(write_blf write_blf.cpp)
target_link_libraries(write_blf blf)

add_executable(write_asc write_asc.cpp)
target_link_libraries(write_asc blf)

add_executable(parser_blf parser_blf.cpp)
target_link_libraries(parser_blf blf)

add_executable(parser_asc parser_asc.cpp)
target_link_libraries(parser_asc blf)

add_executable(blf_to_asc blf_to_asc.cpp)
target_link_libraries(blf_to_asc blf)

add_executable(benchmark_test benchmark.cpp)
target_link_libraries(benchmark_test blf benchmark::benchmark_main)
