#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .combine(std::initializer_list<int>{9, 8, 7, 6, 5, 4, 3, 2, 1}, [](const auto &item1, const auto &item2) {
        return item1 + item2;
        }) == std::vector<int>{10, 10, 10, 10, 10, 10, 10, 10, 10}) {
    return 0;
  }

  return 1;
}
