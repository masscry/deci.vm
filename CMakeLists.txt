cmake_minimum_required(VERSION 3.5)

project(decivm CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(deci
  src/deci.cpp
  src/value_t.cpp
  src/vm_t.cpp
  src/number_t.cpp
  src/stack_t.cpp
  src/program_t.cpp
  src/reference_t.cpp
  src/array_t.cpp

  include/deci.hpp
  include/deci/function_t.hpp
  include/deci/vm_t.hpp
  include/deci/number_t.hpp
  include/deci/func_lib.hpp
  include/deci/value_t.hpp
  include/deci/reference_t.hpp
  include/deci/array_t.hpp
)

target_include_directories(deci
  PRIVATE include/
  SYSTEM INTERFACE include/)

target_compile_options(deci PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
)

add_executable(basic_test
  test/basic_test.cpp
)

target_link_libraries(basic_test PRIVATE deci)
