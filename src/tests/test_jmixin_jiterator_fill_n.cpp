#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>(10))
    .fill_n(2, 1) == std::vector<int>{1, 1, 0, 0, 0, 0, 0, 0, 0, 0}) {
    return 0;
  }

  return 1;
}
