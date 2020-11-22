#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("123.456").is_number() == true) {
    return 0;
  }

  return 1;
}
