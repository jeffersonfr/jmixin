#include "jmixin/jstring.h"

int main()
{
  jmixin::String a("ABC");

  std::string c = a;
  std::string d("DEF");

  c = a;

  jmixin::String e = c;

  if (a > d or d < a) {
    return 1;
  }

  return 0;
}
