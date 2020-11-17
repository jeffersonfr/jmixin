cmake_minimum_required (VERSION 3.0)

project (jmixin)

option(DEBUG "Enable debug strings" ON)
option(TESTS "Enable unit tests" ON)
option(SANITIZE "Enable sanitize" OFF)
option(COVERAGE "Enable coverage" ON)
option(PROFILE "Enable profile" OFF)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
  pkg_check_modules(CAIRO REQUIRED cairo)
add_definitions(${CAIRO_CFLAGS})

if (DEBUG)
  add_definitions("-g3 -ggdb3 -Wall")
endif()

if (SANITIZE)
  add_definitions("-fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize-address-use-after-scope")
endif()

if (COVERAGE)
  add_definitions("-fprofile-arcs -ftest-coverage -fprofile-generate --coverage -fno-inline -O0")
endif()

if (PROFILE)
  add_definitions("-pg")
endif()

enable_testing()

add_subdirectory(src)
