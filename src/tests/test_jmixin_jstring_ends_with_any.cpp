#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.ends_with_any({"world !", "mundo !"}) == true and mixin.ends_with_any({"Hello", "Olá"}) == false) {
    return 0;
  }

  return 1;
}
