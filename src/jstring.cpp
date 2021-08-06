#include "jmixin/jstring.h"
 
namespace jmixin {

  String from_hex(const std::string &str)
  {
    if (str.size() % 2) {
      throw std::runtime_error("Invalid input string");
    }

    String result;

    result.reserve(str.size()/2);

    for (std::size_t i=0; i<str.size(); i+=2) {
      std::size_t nibbleH = str[i + 0];
      std::size_t nibbleL = str[i + 1];

      if (nibbleH > 'A') {
        nibbleH = nibbleH - 'A';
      } else {
        nibbleH = nibbleH - '0';
      }

      if (nibbleL > 'A') {
        nibbleL = nibbleL - 'A';
      } else {
        nibbleL = nibbleL - '0';
      }

      std::size_t value = nibbleH*0x10 + nibbleL*0x01;

      result = result + (char)value;
    }

    return result;
  }

  String latin1_to_utf8(const std::string &str)
  {
    String out;

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
      std::uint8_t ch = *it;

      if (ch < 0x80) {
        out.push_back(ch);
      } else {
        out.push_back(0xc0 | ch >> 6);
        out.push_back(0x80 | (ch & 0x3f));
      }
    }

    return out;
  }

}
