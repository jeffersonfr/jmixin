#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  auto enumerate = jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}).enumerate();

  std::vector<int> result;

  for (const auto &[i, item] : enumerate) {
    result.push_back(i);
  }

  if (result == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}) {
      return 0;
  }

  return 1;
}
