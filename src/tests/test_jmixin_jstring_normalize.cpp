#include "jmixin/jstring.h"

int main()
{
  if (jmixin::String("   Jeff     \t\t \t Ferr de    Araujo \t\t\t\t\tLima    ")
    .normalize() == "Jeff Ferr de Araujo Lima") {
    return 0;
  }

  return 1;
}
