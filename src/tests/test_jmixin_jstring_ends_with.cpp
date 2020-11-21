#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.ends_with("world !") == true and mixin.ends_with("Hello") == false) {
    return 0;
  }

  return 1;
}
