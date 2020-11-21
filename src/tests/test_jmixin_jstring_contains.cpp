#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .contains("world") == true) {
    return 0;
  }

  return 1;
}
