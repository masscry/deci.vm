cmake_minimum_required(VERSION 3.5)

project(decivm CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(deci
  src/deci.cpp
  include/deci.hpp
)

target_include_directories(deci PUBLIC include)

add_executable(basic_test
  test/basic_test.cpp
)

target_link_libraries(basic_test PRIVATE deci)
