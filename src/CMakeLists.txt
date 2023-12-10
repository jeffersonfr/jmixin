cmake_minimum_required (VERSION 3.0)

project (jmixin)

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
  )

if (COVERAGE)
  target_link_libraries(${PROJECT_NAME} PUBLIC
  --coverage
  -lgcov
  )
endif()

install(DIRECTORY include/jmixin DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
