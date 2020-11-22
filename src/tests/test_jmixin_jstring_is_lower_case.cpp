#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("hello, world !").is_lower_case() == true) {
    return 0;
  }

  return 1;
}
