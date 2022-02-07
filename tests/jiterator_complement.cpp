#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 4, 5, 6, 8, 9})
    .sort().complement(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}) == std::vector<int>{3, 7}) {
    return 0;
  }

  return 1;
}
