#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .contains_none({"Olá", "mundo"}) == true) {
    return 0;
  }

  return 1;
}
