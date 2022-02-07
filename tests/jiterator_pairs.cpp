#include "jmixin/jiterator.h"

#include <list>
#include <map>

int main()
{
  if (jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    .pairs(std::initializer_list<int>{10, 20, 30, 40, 50, 60, 70, 80, 90})
    .pairs(std::initializer_list<int>{100, 200, 300, 400, 500, 600, 700, 800, 900}) == std::vector<std::pair<std::pair<int, int>, int>> {
    {{1, 10}, 100}, {{2, 20}, 200}, {{3, 30}, 300}, {{4, 40}, 400}, {{5, 50}, 500}, {{6, 60}, 600}, {{7, 70}, 700}, {{8, 80}, 800}, {{9, 90}, 900}}) {
    return 0;
  }

  return 1;
}
