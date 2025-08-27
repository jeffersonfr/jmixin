cmake_minimum_required (VERSION 3.0)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source build is not allowed")
endif()

project(jmixin
  LANGUAGES CXX
  HOMEPAGE_URL https://github.com/jeffersonfr/jmixin
  DESCRIPTION "A utility classe"
  VERSION 1.0.0
)

option(JMIXIN_PARELELL_FUNCTIONS "Enable paralell algorithms" OFF)
option(JMIXIN_EXTRA_FUNCTIONS "Enable extra functions available in c++20" OFF)
option(JMIXIN_TESTS "Enable unit tests" OFF)
option(JMIXIN_EXAMPLES "Enable examples" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (JMIXIN_PARELELL_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_PARALLEL)
endif()

if (JMIXIN_EXTRA_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_EXTRA)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -pedantic -Werror)

if (JMIXIN_PROFILE)
  add_compile_options(-pg -fprofile-arcs -ftest-coverage -fprofile-generate -fno-inline -O0)
endif()

add_subdirectory(src)

enable_testing()

if (JMIXIN_TESTS)
  add_subdirectory(tests)
endif()

if (JMIXIN_EXAMPLES)
  add_subdirectory(examples)
endif()

# generate pc file
configure_file(jmixin.pc.in jmixin.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jmixin.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
