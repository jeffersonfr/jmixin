cmake_minimum_required (VERSION 3.0)

macro(module_example)
  set(example_file example_${ARGV0})

  add_executable(${example_file} ${example_file}
  )

  add_test(${example_file} ${example_file}
  )

  target_include_directories(${PROJECT_NAME} PUBLIC
    ../src/include
  )

  target_link_libraries(${example_file} PUBLIC
    ${PROJECT_NAME}
    pthread
    tbb
  )

  if (SANITIZE)
    target_link_libraries(${example_file} PUBLIC
      -fsanitize=address
      -fsanitize=address
      -fsanitize=leak
      -fsanitize=undefined
      -fsanitize-address-use-after-scope
    )
  endif()
  
  if (COVERAGE)
    target_link_libraries(${example_file} PUBLIC
      --coverage 
      -lgcov
    )
  endif()

  if (PROFILE)
    add_definitions("-pg")
  endif()
endmacro()

module_example(megasena)
module_example(process_info)
module_example(ffmpeg_info)
module_example(certidao_casamento)
