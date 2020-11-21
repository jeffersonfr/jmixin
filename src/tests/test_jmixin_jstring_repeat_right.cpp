#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world")
    .repeat_right("!", 5) == "Hello, world!!!!!") {
    return 0;
  }

  return 1;
}
