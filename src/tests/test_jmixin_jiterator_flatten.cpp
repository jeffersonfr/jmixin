#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<std::list<int>>{{1, 2}, {3, 4}, {5, 6}, {7, 8}})
    .flatten() == std::list<int>{1, 2, 3, 4, 5, 6, 7, 8}) {
    return 0;
  }

  return 1;
}
