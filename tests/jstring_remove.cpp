#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .remove(", world !") == "Hello") {
    return 0;
  }

  return 1;
}
