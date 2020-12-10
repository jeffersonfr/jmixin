#include "jmixin/jiterator.h"
#include "jmixin/jstring.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .build<std::vector<jmixin::String<std::string>>>([](const auto &thiz) {
      return jmixin::String("hello").repeat(thiz.size(), ",").split(",");
    }) == std::vector<jmixin::String<std::string>>{"hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello", "hello"}) {
    return 0;
  }

  return 1;
}
