#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world")
    .left_repeat("!", 5) == "!!!!!Hello, world") {
    return 0;
  }

  return 1;
}
