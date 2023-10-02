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

option(PARELELL_FUNCTIONS "Enable paralell algorithms" OFF)
option(EXTRA_FUNCTIONS "Enable extra functions available in c++20" OFF)
option(TESTS "Enable unit tests" OFF)
option(EXAMPLES "Enable examples" OFF)
option(SANITIZE "Enable sanitize" OFF)
option(COVERAGE "Enable coverage" OFF)
option(PROFILE "Enable profile" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (PARELELL_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_PARALLEL)
endif()

if (EXTRA_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_EXTRA)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -pedantic -Werror)

if (COVERAGE)
  add_compile_options(--coverage)
  add_link_options(-lgcov)
endif()

if (SANITIZE)
  set(SANITIZE_FLAGS -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize-address-use-after-scope -U_FORTIFY_SOURCE -fno-omit-frame-pointer -fno-common)

  add_compile_options(${SANITIZE_FLAGS})
  add_link_options(${SANITIZE_FLAGS} -lasan -lubsan)
endif()

if (PROFILE)
  add_compile_options(-pg -fprofile-arcs -ftest-coverage -fprofile-generate -fno-inline -O0)
endif()

enable_testing()

add_subdirectory(src)

if (TESTS)
  add_subdirectory(tests)
endif()

if (EXAMPLES)
  add_subdirectory(examples)
endif()

# generate pc file
configure_file(jmixin.pc.in jmixin.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/jmixin.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
