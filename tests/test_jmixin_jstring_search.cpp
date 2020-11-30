#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world ! We living in a beautiful world !")
    .search("world").size() == 2) {
    return 0;
  }

  return 1;
}
