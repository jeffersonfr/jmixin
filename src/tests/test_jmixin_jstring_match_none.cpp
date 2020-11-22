#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("Hello, world !");

  if (mixin.match_none({"Olá, mundo !", "Bom dia !"}) == true) {
    return 0;
  }

  return 1;
}
