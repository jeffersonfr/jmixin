#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.starts_with_any({"Hello", "Olá"}) == true and mixin.starts_with_any({"world", "mundo"}) == false) {
    return 0;
  }

  return 1;
}
