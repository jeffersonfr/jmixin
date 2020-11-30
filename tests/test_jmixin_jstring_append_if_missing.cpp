#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("file1.txt")
    .append_if_missing(".txt") == "file1.txt") {
    return 0;
  }

  if (jmixin::String("file2")
    .append_if_missing(".txt") == "file2.txt") {
    return 0;
  }

  return 1;
}
