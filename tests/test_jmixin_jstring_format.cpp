#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("%d => %d").format(10, 20) == "10 => 20") {
    return 0;
  }

  return 1;
}
