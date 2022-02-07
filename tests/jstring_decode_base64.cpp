#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !").encode_base64().decode_base64() == "Hello, world !") {
    return 0;
  }

  return 1;
}
