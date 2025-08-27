cmake_minimum_required (VERSION 3.0)

project (jmixin)

include(FetchContent)

# fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
)
FetchContent_MakeAvailable(fmt)

add_library(${PROJECT_NAME} SHARED
  jiterator.cpp
  jstring.cpp
  )

include_directories(
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  fmt
  )

if (JMIXIN_COVERAGE)
  target_link_libraries(${PROJECT_NAME} PUBLIC
  --coverage
  -lgcov
  )
endif()

install(DIRECTORY include/jmixin DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
