#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  std::vector<int> result;

  jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .for_each([&result](const auto &item) {
        std::cout << item << " " << std::endl;

        result.push_back(item);
    });

  if (result == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    return 0;
  }

  return 1;
}
