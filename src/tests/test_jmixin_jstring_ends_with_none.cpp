#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !").ends_with_none({"mundo !", "planet !"}) == true) {
    return 0;
  }

  return 1;
}
