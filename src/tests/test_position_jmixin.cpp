#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .position([](const auto &item) {
        if (item == 6) {
          return true;
        }

        return false;
    }) == 5) {
    return 0;
  }

  return 1;
}
