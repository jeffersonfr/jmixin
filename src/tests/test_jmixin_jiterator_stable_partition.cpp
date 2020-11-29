#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .stable_partition([](const auto &item) {
        return (item % 2 == 0);
    }) == std::vector<std::vector<int>>{{2, 4, 6, 8}, {1, 3, 5, 7, 9}}) {
    return 0;
  }

  return 1;
}
