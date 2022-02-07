#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.starts_with("Hello") == true and mixin.starts_with("world") == false) {
    return 0;
  }

  return 1;
}
