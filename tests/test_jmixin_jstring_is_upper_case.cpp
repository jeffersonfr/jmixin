#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("HELLO, WORLD !").is_upper_case() == true) {
    return 0;
  }

  return 1;
}
