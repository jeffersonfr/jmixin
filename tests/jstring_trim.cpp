#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("   Hello, world !   ")
    .trim() == "Hello, world !") {
    return 0;
  }

  return 1;
}
