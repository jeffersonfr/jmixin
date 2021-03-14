cmake_minimum_required (VERSION 3.0)

project (jmixin)

option(PARELELL_FUNCTIONS "Enable paralell algorithms" OFF)
option(EXTRA_FUNCTIONS "Enable extra functions available in c++20" OFF)
option(TESTS "Enable unit tests" OFF)
option(EXAMPLES "Enable examples" OFF)
option(SANITIZE "Enable sanitize" OFF)
option(COVERAGE "Enable coverage" OFF)
option(PROFILE "Enable profile" OFF)

set(CMAKE_CXX_STANDARD 17)

if (PARELELL_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_PARALLEL)
endif()

if (EXTRA_FUNCTIONS)
  add_definitions(-DJMIXIN_JITERATOR_EXTRA)
endif()

if (SANITIZE)
  set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize-address-use-after-scope"
  )
endif()

if (COVERAGE)
  set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} --coverage -lgcov"
  )
endif()

if (PROFILE)
  set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -pg -fprofile-arcs -ftest-coverage -fprofile-generate -fno-inline -O0"
  )
  set(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} -pg"
  )
endif()

add_subdirectory(src)

enable_testing()

if (TESTS)
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
