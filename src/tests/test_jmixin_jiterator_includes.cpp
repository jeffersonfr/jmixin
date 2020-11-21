#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{6, 9, 5, 8, 3, 1, 7, 4, 2})
    .sort().includes(std::vector<int>{3, 4, 5}) == true) {
    return 0;
  }

  return 1;
}
