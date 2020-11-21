#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .none([](const auto &item) {
        if (item > 10) {
          return true;
        }

        return false;
    }) == true) {
    return 0;
  }

  return 1;
}
