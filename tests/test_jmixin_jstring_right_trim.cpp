#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("   Hello, world !   ")
    .right_trim() == "   Hello, world !") {
    return 0;
  }

  return 1;
}
