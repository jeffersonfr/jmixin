#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .find_last([](const auto &item) {
        if (item % 2) {
            return true;
        }

        return false;
        }) == 9) {
    return 0;
  }

  return 1;
}
