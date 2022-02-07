#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.match("Hello, world !") == true and mixin.match("Hello") == false) {
    return 0;
  }

  return 1;
}
