#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .map<std::map<int, int>>([i=0](const auto &item) mutable {
      return std::make_pair(i++, item);
    }).map<std::list<std::string>>([](const auto &item) {
      return std::to_string(item.first) + "." + std::to_string(item.second);
    }) == std::list<std::string>{"0.1", "1.2", "2.3", "3.4", "4.5", "5.6", "6.7", "7.8", "8.9"}) {
    return 0;
  }

  return 1;
}
