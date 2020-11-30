#include "jmixin/jstring.h"

int main()
{
  /*
  std::cout << "|" << jmixin::String("name").align(jmixin::align::center, 32) << "|" << jmixin::String("address").align(jmixin::align::center, 32) << "|" << jmixin::String("phone").align(jmixin::align::center, 16) << "|" << std::endl;
  std::cout << "|" << jmixin::String("-").repeat(32) << "|" << jmixin::String("-").repeat(32) << "|" << jmixin::String("-").repeat(16) << "|" << std::endl;
  std::cout << "|" << jmixin::String("Jeff Ferr").align(jmixin::align::right, 32) << "|" << jmixin::String("R. Jose C. Chavez").align(jmixin::align::right, 32) << "|" << jmixin::String("83 9 9680 4040").align(jmixin::align::right, 16) << "|" << std::endl;
  std::cout << "|" << jmixin::String("Talita Paz").align(jmixin::align::right, 32) << "|" << jmixin::String("R. F. Mendes S.").align(jmixin::align::right, 32) << "|" << jmixin::String("83 9 9918 9100").align(jmixin::align::right, 16) << "|" << std::endl;
  std::cout << "|" << jmixin::String("-").repeat(32) << "|" << jmixin::String("-").repeat(32) << "|" << jmixin::String("-").repeat(16) << "|" << std::endl;
  */

  if (jmixin::String("Hello, world !")
    .align(jmixin::align::left, 16) == "Hello, world !  ") {
    return 0;
  }

  if (jmixin::String("Hello, world !")
    .align(jmixin::align::right, 16) == "  Hello, world !") {
    return 0;
  }

  if (jmixin::String("Hello, world !")
    .align(jmixin::align::center, 16) == " Hello, world ! ") {
    return 0;
  }

  return 1;
}
