#include "jmixin/jstring.h"

#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::clog << "usage: " << argv[0] << " <number>" << std::endl;

    return 0;
  }

  std::string str {argv[1]};

  jmixin::String code(str);

  if (code.size() != 32) {
    std::cout << "code must have 32 chars" << std::endl;

    return 0;
  }

  std::cout << "cartorio: " << code.substr(0, 6) << std::endl;
  std::cout << "acervo: " << code.substr(6, 2) << std::endl;
  std::cout << "numero de servico de registro civil: " << code.substr(8, 2) << std::endl;
  std::cout << "ano: " << code.substr(10, 4) << std::endl;
  std::cout << "tipo: " << code.substr(14, 1) << std::endl;
  std::cout << "livro: " << code.substr(15, 5) << std::endl;
  std::cout << "folha: " << code.substr(20, 3) << std::endl;
  std::cout << "termo: " << code.substr(23, 7) << std::endl;

  return 0;
}

