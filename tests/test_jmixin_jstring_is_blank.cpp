#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("  \t \t \t \t  ").is_blank() == true) {
    return 0;
  }

  return 1;
}
