#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .iterator().chop(2) == "Hello, world") {
    return 0;
  }

  return 1;
}
