#include "jmixin/jstring.h"

#include <list>
#include <map>

int main()
{
  std::cout << jmixin::String("Hello, world !")
    .upper_case() << std::endl;

  return 0;
}

