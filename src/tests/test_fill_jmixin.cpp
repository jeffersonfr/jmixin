#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>(10))
    .fill(1) == std::vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}) {
    return 0;
  }

  return 1;
}
