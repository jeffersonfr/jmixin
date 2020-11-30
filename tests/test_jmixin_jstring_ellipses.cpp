#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .ellipses(8) == "Hello...") {
    return 0;
  }

  return 1;
}
