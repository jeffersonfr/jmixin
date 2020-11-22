#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("hello123").is_alpha_numeric() == true) {
    return 0;
  }

  return 1;
}
