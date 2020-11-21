#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .replace("world", "beautiful world") == "Hello, beautiful world !") {
    return 0;
  }

  return 1;
}
