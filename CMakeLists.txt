cmake_minimum_required (VERSION 3.0)

project (jmixin)

option(DEBUG "Enable debug strings" ON)
option(TESTS "Enable unit tests" ON)
option(EXAMPLES "Enable examples" ON)
option(SANITIZE "Enable sanitize" OFF)
option(COVERAGE "Enable coverage" ON)
option(PROFILE "Enable profile" OFF)

set(CMAKE_CXX_STANDARD 20)

if (SANITIZE)
  add_definitions("-fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize-address-use-after-scope")
endif()

if (COVERAGE)
  add_definitions("-fprofile-arcs -ftest-coverage -fprofile-generate --coverage -fno-inline -O0")
endif()

if (PROFILE)
  add_definitions("-pg")
endif()

add_subdirectory(src)

if (TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if (EXAMPLES)
  add_subdirectory(examples)
endif()

# generate jmixin.pc
configure_file(
  ${PROJECT_SOURCE_DIR}/jmixin.pc.in
  ${PROJECT_BINARY_DIR}/jmixin.pc
  @ONLY
)

install(FILES ${CMAKE_BINARY_DIR}/jmixin.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
