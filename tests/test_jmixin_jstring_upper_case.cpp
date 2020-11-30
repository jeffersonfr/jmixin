#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, World !")
    .upper_case() == "HELLO, WORLD !") {
    return 0;
  }

  return 1;
}
