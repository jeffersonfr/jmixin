#pragma once

#include "jiterator.h"

#include <algorithm>
#include <optional>
#include <regex>

namespace jmixin {

  enum class align {
    left,
    center,
    right
  };

  class String : public std::string {

    public:
      String(const char *str):
        std::string(str)
      {
      }

      String(const std::string &str):
        std::string(str)
      {
      }

      Iterator<String> iterator()
      {
        return Iterator<String>(*this);
      }

      String lower_case()
      {
        std::for_each(std::begin(*this), std::end(*this), [](auto &ch) {
          ch = tolower(ch);
        });

        return *this;
      }

      String upper_case()
      {
        std::for_each(std::begin(*this), std::end(*this), [](auto &ch) {
          ch = toupper(ch);
        });

        return *this;
      }

      String swap_case()
      {
        std::for_each(std::begin(*this), std::end(*this), [](auto &ch) {
          if (ch == toupper(ch)) {
            ch = tolower(ch);
          } else {
            ch = toupper(ch);
          }
        });

        return *this;
      }

      String captalize()
      {
        for (auto &ch : *this) {
          if (isalpha(ch) != 0) {
            ch = toupper(ch);

            break;
          }
        }

        return *this;
      }

      String uncaptalize()
      {
        for (auto &ch : *this) {
          if (isalnum(ch) != 0) {
            ch = tolower(ch);

            break;
          }
        }

        return *this;
      }

      String left_trim()
      {
        const std::regex r{"^[[:space:]]*", std::regex_constants::extended};

        return std::regex_replace(*this, r, "");
      }

      String right_trim()
      {
        const std::regex r{"[[:space:]]*$", std::regex_constants::extended};

        return std::regex_replace(*this, r, "");
      }

      String trim()
      {
        return left_trim().right_trim();
      }

      String replace(const String &pattern, const String value)
      {
        const std::regex r{pattern, std::regex_constants::extended};

        return std::regex_replace(*this, r, value);
      }

      bool match(const String &pattern)
      {
        const std::regex r{pattern};

        return std::regex_match(*this, r);
      }

      Iterator<std::vector<String>> search(const String &pattern)
      {
        const std::regex r(pattern);
        
        std::vector<String> result;
        std::smatch matches;
        std::string str = *this;

        while (regex_search(str, matches, r)) {
          result.push_back(matches[0].str());

          str = matches.suffix();
        }

        return Iterator{result};
      }

      bool contains(String value)
      {
        if (this->find(value) != std::string::npos) {
          return true;
        }

        return false;
      }

      bool starts_with(const String &value)
      {
        if (std::find_first_of(std::begin(*this), std::end(*this), std::begin(value), std::end(value)) == std::begin(*this)) {
          return true;
        }

        return false;
      }

      bool ends_with(const String &value)
      {
        if (static_cast<std::size_t>(std::distance(std::begin(*this), std::find_end(std::begin(*this), std::end(*this), std::begin(value), std::end(value)))) == value.size()) {
          return true;
        }

        return false;
      }

      String & append_if_missing(const String &value)
      {
        if (this->ends_with(value) == false) {
          *this = *this + value;
        }

        return *this;
      }

      String & prepend_if_missing(const String &value)
      {
        if (this->starts_with(value) == false) {
          *this = value + *this;
        }

        return *this;
      }

      String repeat(std::size_t n)
      {
        std::string result;

        result.reserve(this->size()*n);

        for (std::size_t i=0; i<n; i++) {
          result = result + *this;
        }

        return String{result};
      }

      String repeat_left(const String &value, std::size_t n)
      {
        std::string result;

        result.reserve(this->size() + value.size() + n);

        for (std::size_t i=0; i<n; i++) {
          result = result + value;
        }

        result = result + *this;

        return String{result};
      }

      String repeat_right(const String &value, std::size_t n)
      {
        std::string result;

        result.reserve(this->size() + value.size() + n);

        result = *this;

        for (std::size_t i=0; i<n; i++) {
          result = result + value;
        }

        return String{result};
      }

      String align(align align, std::size_t length, const char fill = ' ')
      {
        if (this->size() >= length) {
          return *this;
        }

        std::size_t padding = length - this->size();

        if (align == align::left) {
          return *this + std::string(padding, fill);
        } else if (align == align::right) {
          return std::string(padding, fill) + *this;
        }

        return std::string(padding/2, fill) + *this + std::string(padding/2 + padding%2, fill);
      }

      String ellipses(std::size_t length, const String &value = String("..."))
      {
        if (this->size() < length) {
          return *this;
        }

        return this->substr(0, length - value.size()) + value;
      }

      Iterator<std::vector<String>> split(const String &pattern = String("\\s+"))
      {
          std::vector<String> result;

          std::regex r(pattern);
          std::sregex_token_iterator it(std::begin(*this), std::end(*this), r, -1);
          std::sregex_token_iterator end;

          while (it != end)  {
            result.push_back(String{*it++});
          }

          return Iterator{result};
        }

      String remove(const String &value)
      {
        return this->replace(value, "");
      }

      /* -std=c++20 still not working
      template<typename ...Args>
      String format(Args &&...args)
      {
        return std::format(*this, std::forward<Args>(args)...);
      }
      */

  };

}
