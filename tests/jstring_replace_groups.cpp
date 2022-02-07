#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .replace_groups("(world)", "beautiful $1") == "Hello, beautiful world !") {
    return 0;
  }

  return 1;
}
