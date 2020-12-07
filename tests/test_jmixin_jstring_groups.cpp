#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("Hello, world !")
    .groups("(Hello), (world)")[0].skip(1) == std::vector<jmixin::String>{"Hello", "world"}) {
    return 0;
  }

  return 1;
}
