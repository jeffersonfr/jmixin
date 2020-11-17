cmake_minimum_required (VERSION 3.0)

macro(module_test)
  add_executable(test_${ARGV0}_${PROJECT_NAME}
    test_${ARGV0}_${PROJECT_NAME}.cpp
  )

  add_test(test_${ARGV0}_${PROJECT_NAME} test_${ARGV0}_${PROJECT_NAME}
  )

  target_link_libraries(test_${ARGV0}_${PROJECT_NAME} PUBLIC
    ${PROJECT_NAME}
  )

  if (SANITIZE)
    target_link_libraries(test_${ARGV0}_${PROJECT_NAME} PUBLIC
      -fsanitize=address
      -fsanitize=address
      -fsanitize=leak
      -fsanitize=undefined
      -fsanitize-address-use-after-scope
    )
  endif()
  
  if (COVERAGE)
    target_link_libraries(test_${ARGV0}_${PROJECT_NAME} PUBLIC
      --coverage 
      -lgcov
    )
  endif()
endmacro()

module_test(eq)
module_test(ne)
module_test(lt)
module_test(le)
module_test(gt)
module_test(ge)
module_test(foreach)
module_test(sort)
module_test(filter)
module_test(count)
module_test(step)
module_test(sum)
module_test(product)
module_test(zip)
module_test(map)
module_test(fill)
module_test(skip)
module_test(take)
module_test(reverse)
module_test(shuffle)
module_test(sample)
module_test(lrotate)
module_test(rrotate)
module_test(enumerate)
module_test(swap)
module_test(first)
module_test(last)
module_test(nth)
module_test(find)
module_test(find_last)
module_test(min)
module_test(max)
module_test(position)
module_test(all)
module_test(any)
module_test(none)
module_test(includes)
module_test(difference)
module_test(intersection)
module_test(union_set)
module_test(unique)
module_test(append)
module_test(merge)
module_test(combine)
module_test(pairs)
