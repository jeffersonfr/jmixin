#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{8, 7, 1, 2, 1, 3, 4, 3, 3, 5, 6, 7, 4, 8, 9})
    .sort().unique() == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    return 0;
  }

  return 1;
}
