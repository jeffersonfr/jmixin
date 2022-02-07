#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, World !")
    .lower_case() == "hello, world !") {
    return 0;
  }

  return 1;
}
