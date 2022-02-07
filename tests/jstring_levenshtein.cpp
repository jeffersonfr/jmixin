#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !").levenshtein("Helo, world !") == 1) {
    return 0;
  }

  return 1;
}
