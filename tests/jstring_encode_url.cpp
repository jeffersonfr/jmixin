#include "jmixin/jstring.h"

int main()
{
  std::cout << jmixin::String("Hello, world !").encode_url() << std::endl;
  if (jmixin::String("Hello, world !").encode_url() == "Hello%2c%20world%20%21") { 
    return 0;
  }

  return 1;
}
