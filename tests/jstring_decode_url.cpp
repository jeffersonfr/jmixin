#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello%2C%20world%20%21").decode_url() == "Hello, world !") {
    return 0;
  }

  return 1;
}
