cmake_minimum_required (VERSION 3.0)

find_package(Threads REQUIRED)

macro(module_test)
  add_executable(${ARGV0}_example
    ${ARGV0}.cpp
  )

  target_link_libraries(${ARGV0}_example
    PRIVATE
      jmixin
  )

  if (PARELELL)
    target_link_libraries(${ARGV0}_example
      PRIVATE
        tbb
    )
  endif()
endmacro()

#module_test(megasena)
module_test(process_info)
module_test(ffmpeg_info)
module_test(certidao_casamento)
