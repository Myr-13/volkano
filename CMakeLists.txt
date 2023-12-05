cmake_minimum_required(VERSION 3.20)
project(Volkano)

file(GLOB VOLKANO_SOURCE "src/volkano/*.cpp")
add_library(volkano ${VOLKANO_SOURCE})
target_include_directories(volkano PRIVATE include)

add_executable(volkano_test src/tests/main.cpp)
target_include_directories(volkano_test PRIVATE include)
target_link_libraries(volkano_test volkano)

add_executable(vlk_decompiler src/tools/decompiler.cpp)
target_include_directories(vlk_decompiler PRIVATE include src)
target_link_libraries(vlk_decompiler volkano)
