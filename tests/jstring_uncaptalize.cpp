#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, World !")
    .uncaptalize() == "hello, World !") {
    return 0;
  }

  return 1;
}
