#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("one tow three four")
    .split("\\s+").size() == 4) {
    return 0;
  }

  return 1;
}
