cmake_minimum_required(VERSION 3.10.0)


add_executable(proj_test test.cpp lib.cpp position.cpp searcher.cpp)
target_link_libraries(proj_test PUBLIC doctest::doctest default)
doctest_discover_tests(proj_test)

add_executable(console console.cpp lib.cpp position.cpp searcher.cpp)
target_link_libraries(console PUBLIC default)

add_executable(window main.cpp lib.cpp position.cpp searcher.cpp)
target_link_libraries(window PUBLIC default CONAN_PKG::raylib raylib-cpp)