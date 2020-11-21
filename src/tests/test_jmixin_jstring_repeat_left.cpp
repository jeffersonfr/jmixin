#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world")
    .repeat_left("!", 5) == "!!!!!Hello, world") {
    return 0;
  }

  return 1;
}
