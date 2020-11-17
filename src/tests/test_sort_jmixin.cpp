#include "jmixin/jiterator.h"

int main()
{
  if (jmixin::Iterator(std::vector<int>{5, 9, 1, 6, 7, 8, 3, 2, 4})
    .sort() == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    return 0;
  }

  return 1;
}
