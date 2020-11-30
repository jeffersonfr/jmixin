#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("hello, world !")
    .captalize() == "Hello, world !") {
    return 0;
  }

  return 1;
}
