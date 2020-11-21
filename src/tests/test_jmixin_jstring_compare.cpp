#include "jmixin/jstring.h"

int main()
{
  auto mixin = jmixin::String("abcd");

  if (mixin.compare("bbcd") < 0 and mixin.compare("aacd") > 0 and mixin.compare("abcd") == 0) {
    return 0;
  }

  return 1;
}
