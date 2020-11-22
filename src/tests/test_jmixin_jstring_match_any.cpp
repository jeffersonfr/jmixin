#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.match_any({"Hello, world !", "Olá, mundo !"}) == true) {
    return 0;
  }

  return 1;
}
