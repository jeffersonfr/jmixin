#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .contains_any({"world", "mundo"}) == true) {
    return 0;
  }

  return 1;
}
