#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("ABC").hex() == "414243") {
    return 0;
  }

  return 1;
}
