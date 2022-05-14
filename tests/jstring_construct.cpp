#include "jmixin/jstring.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  char str1[] = "Hello, world !";
  char str2[15] = "Hello, world !";

  std::cout << jmixin::String(std::string(str1)) << std::endl;
  std::cout << jmixin::String(std::string(str2)) << std::endl;
  std::cout << jmixin::String("Hello, world !") << std::endl;

  return 0;
}
