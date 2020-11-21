#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .iterator().count([](const auto &ch) {
        if (ch == 'l') {
          return true;
        }

        return false;
    }) == 3) {
    return 0;
  }

  return 1;
}
