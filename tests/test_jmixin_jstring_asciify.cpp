#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Çedil").asciify() == "Cedil") {
    return 0;
  }

  return 1;
}
