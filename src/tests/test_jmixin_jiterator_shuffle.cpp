#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (auto result = jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .shuffle(); result != std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9} and jmixin::Iterator(result).sum<int>() == 45) {
    return 0;
  }

  return 1;
}
