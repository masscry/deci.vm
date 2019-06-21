cmake_minimum_required(VERSION 3.5)

project(decivm CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
  IEC61131_PARSER src/iec61131.y
  ${CMAKE_CURRENT_BINARY_DIR}/iec61131.y.cpp
  COMPILE_FLAGS --no-lines
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/iec61131.y.hpp
)

FLEX_TARGET(
  IEC61131_SCANNER src/iec61131.l 
  ${CMAKE_CURRENT_BINARY_DIR}/iec61131.l.cpp
)

ADD_FLEX_BISON_DEPENDENCY(IEC61131_SCANNER IEC61131_PARSER)

add_library(deci
  src/deci.cpp
  src/value_t.cpp
  src/vm_t.cpp
  src/number_t.cpp
  src/stack_t.cpp
  src/program_t.cpp
  src/reference_t.cpp
  src/array_t.cpp
  src/dictionary_t.cpp
  src/compiler_t.cpp

  include/deci.hpp
  include/deci/function_t.hpp
  include/deci/vm_t.hpp
  include/deci/number_t.hpp
  include/deci/func_lib.hpp
  include/deci/value_t.hpp
  include/deci/reference_t.hpp
  include/deci/array_t.hpp
  include/deci/dictionary_t.hpp
  include/deci/compiler_t.hpp

  ${BISON_IEC61131_PARSER_OUTPUTS}
  ${FLEX_IEC61131_SCANNER_OUTPUTS}
)

target_include_directories(deci
  PRIVATE include/ ${CMAKE_CURRENT_BINARY_DIR}
  SYSTEM INTERFACE include/)

target_compile_options(deci PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
)

add_executable(basic_test
  test/basic_test.cpp
)

target_link_libraries(basic_test PRIVATE deci)

add_executable(values_test
  test/values_test.cpp
)

target_link_libraries(values_test PRIVATE deci)

add_executable(compile_test
  test/compile_test.cpp
)

target_link_libraries(compile_test PRIVATE deci)
