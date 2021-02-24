cmake_minimum_required (VERSION 3.0)

project (jmixin)

add_library(${PROJECT_NAME} SHARED
  jiterator.cpp
  jstring.cpp
  )

include_directories(
  "${CMAKE_SOURCE_DIR}/src/include"
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  )

if (COVERAGE)
  target_link_libraries(${PROJECT_NAME} PUBLIC
  --coverage
  -lgcov
  )
endif()

install(DIRECTORY include/jmixin DESTINATION /usr/local/include)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /usr/local/lib)
