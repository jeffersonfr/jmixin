#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, World !")
    .swap_case() == "hELLO, wORLD !") {
    return 0;
  }

  return 1;
}
