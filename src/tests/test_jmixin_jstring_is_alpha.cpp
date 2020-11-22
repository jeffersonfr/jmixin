#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("helloworld").is_alpha() == true) {
    return 0;
  }

  return 1;
}
