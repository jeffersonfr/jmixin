#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !").encode_base64() == "SGVsbG8sIHdvcmxkICE=") {
    return 0;
  }

  return 1;
}
