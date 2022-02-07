#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("https://url")
    .prepend_if_missing("https://") == "https://url") {
    return 0;
  }

  if (jmixin::String("url")
    .prepend_if_missing("https://") == "https://url") {
    return 0;
  }

  return 1;
}
