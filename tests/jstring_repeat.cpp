#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .repeat(3) == "Hello, world !Hello, world !Hello, world !") {
    return 0;
  }

  return 1;
}
