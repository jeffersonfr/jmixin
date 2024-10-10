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
option(JMIXIN_SANITIZE "Enable sanitize" OFF)
option(JMIXIN_COVERAGE "Enable coverage" OFF)
option(JMIXIN_PROFILE "Enable profile" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (JMIXIN_PARELELL_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_PARALLEL)
endif()

if (JMIXIN_EXTRA_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_EXTRA)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -pedantic -Werror)

if (JMIXIN_COVERAGE)
  add_compile_options(--coverage)
  add_link_options(-lgcov)
endif()

if (JMIXIN_SANITIZE)
  set(SANITIZE_FLAGS -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize-address-use-after-scope -U_FORTIFY_SOURCE -fno-omit-frame-pointer -fno-common)

  add_compile_options(${SANITIZE_FLAGS})
  add_link_options(${SANITIZE_FLAGS} -lasan -lubsan)
endif()

if (JMIXIN_PROFILE)
  add_compile_options(-pg -fprofile-arcs -ftest-coverage -fprofile-generate -fno-inline -O0)
endif()

enable_testing()

add_subdirectory(src)

if (JMIXIN_TESTS)
  add_subdirectory(tests)
endif()

if (JMIXIN_EXAMPLES)
  add_subdirectory(examples)
endif()

# generate pc file
configure_file(jmixin.pc.in jmixin.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/jmixin.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
