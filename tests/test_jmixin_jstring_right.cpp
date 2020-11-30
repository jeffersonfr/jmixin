#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .right(16) == "  Hello, world !") {
    return 0;
  }

  return 1;
}
