#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .sort().difference(std::vector<int>{4, 7, 8}) == std::vector<int>{1, 2, 3, 5, 6, 9}) {
    return 0;
  }

  return 1;
}
