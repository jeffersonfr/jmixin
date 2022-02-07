#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .iterator().chop(2).lower_case() == "hello, world") {
    return 0;
  }

  return 1;
}
