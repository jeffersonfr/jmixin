#include "jmixin/jstring.h"

int main()
{
  if (jmixin::join(std::initializer_list<jmixin::String>{"a", "b", "c", "d"}) == "a,b,c,d") {
    return 0;
  }

  return 1;
}
