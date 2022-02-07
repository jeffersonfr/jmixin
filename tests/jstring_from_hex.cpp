#include "jmixin/jstring.h"

int main()
{
  if (jmixin::from_hex("414243") == "ABC") {
    return 0;
  }

  return 1;
}
