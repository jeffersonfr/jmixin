#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .reverse() == std::vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1}) {
    return 0;
  }

  return 1;
}
