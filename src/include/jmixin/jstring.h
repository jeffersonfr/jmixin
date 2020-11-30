#pragma once

#include "jiterator.h"

#include <algorithm>
#include <optional>
#include <regex>
#include <initializer_list>
#include <set>

namespace jmixin {

  enum class align {
    left,
    center,
    right
  };

  class String : public std::string {

    public:
      String() = default;

      String(const char *str):
        std::string(str)
      {
      }

      String(const std::string &str):
        std::string(str)
      {
      }

      Iterator<String> iterator() const
      {
        return Iterator<String>(*this);
      }

      String & lower_case()
      {
        std::for_each(std::begin(*this), std::end(*this), [](auto &ch) {
          ch = tolower(ch);
        });

        return *this;
      }

      String & upper_case()
      {
        std::for_each(std::begin(*this), std::end(*this), [](auto &ch) {
          ch = toupper(ch);
        });

        return *this;
      }

      String & swap_case()
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

      String & captalize()
      {
        for (auto &ch : *this) {
          if (isalpha(ch) != 0) {
            ch = toupper(ch);

            break;
          }
        }

        return *this;
      }

      String & uncaptalize()
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

      String replace(const String &pattern, const String &value)
      {
        const std::regex r{pattern, std::regex_constants::nosubs | std::regex_constants::extended};

        return std::regex_replace(*this, r, value);
      }

      String replace_groups(const String &pattern, const String &format, std::regex_constants::match_flag_type flags = std::regex_constants::match_default)
      {
        const std::regex r{pattern, std::regex_constants::extended};

        return std::regex_replace(*this, r, format, flags);
      }

      bool match(const String &pattern, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        const std::regex r{pattern, flags};

        return std::regex_match(*this, r);
      }

      bool match_any(std::initializer_list<String> patterns, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        for (auto &pattern : patterns) {
          if (match(pattern, flags) == true) {
            return true;
          }
        }

        return false;
      }

      bool match_none(std::initializer_list<String> patterns, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        for (auto &pattern : patterns) {
          if (match(pattern, flags) == true) {
            return false;
          }
        }

        return true;
      }

      Iterator<std::vector<String>> search(const String &pattern, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        const std::regex r(pattern, flags);
        
        std::vector<String> result;
        std::smatch matches;
        std::string str = *this;

        while (regex_search(str, matches, r)) {
          result.push_back(matches[0].str());

          str = matches.suffix();
        }

        return Iterator{result};
      }

      bool contains(const String &pattern, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        if (search(pattern, flags).size() > 0) {
          return true;
        }

        return false;
      }

      bool contains_any(std::initializer_list<String> values, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        for (auto &value : values) {
          if (contains(value, flags) == true) {
            return true;
          }
        }

        return false;
      }

      bool contains_none(std::initializer_list<String> values, std::regex_constants::syntax_option_type flags = std::regex_constants::icase) const
      {
        for (auto &value : values) {
          if (contains(value, flags) == true) {
            return false;
          }
        }

        return true;
      }

      bool starts_with(const String &value) const
      {
        if (value.size() > this->size()) {
          return false;
        }

        std::string::const_iterator i = std::begin(*this);
        std::string::const_iterator j = std::cbegin(value);

        for (; i!=std::end(*this) and j!=std::cend(value);) {
          if (*i++ != *j++) {
            return false;
          }
        }

        return true;
      }

      bool starts_with_any(std::initializer_list<String> values) const {
        for (auto &value : values) {
          if (starts_with(value) == true) {
            return true;
          }
        }

        return false;
      }

      bool starts_with_none(std::initializer_list<String> values) const {
        for (auto &value : values) {
          if (starts_with(value) == true) {
            return false;
          }
        }

        return true;
      }

      bool ends_with(const String &value) const
      {
        if (value.size() > this->size()) {
          return false;
        }

        std::string::const_iterator i = std::begin(*this) + std::size(*this) - std::size(value);
        std::string::const_iterator j = std::cbegin(value);

        for (; i!=std::end(*this) and j!=std::end(value);) {
          if (*i++ != *j++) {
            return false;
          }
        }

        return true;
      }

      bool ends_with_any(std::initializer_list<String> values) const {
        for (auto &value : values) {
          if (ends_with(value) == true) {
            return true;
          }
        }

        return false;
      }

      bool ends_with_none(std::initializer_list<String> values) const {
        for (auto &value : values) {
          if (ends_with(value) == true) {
            return false;
          }
        }

        return true;
      }

      String & append_if_missing(const String &value)
      {
        if (contains(value + "$") == false) {
          *this = *this + value;
        }

        return *this;
      }

      String & prepend_if_missing(const String &value)
      {
        if (contains("^" + value) == false) {
          *this = value + *this;
        }

        return *this;
      }

      String repeat(std::size_t n, const String &aggregator = {}) const
      {
        std::string result;

        result.reserve(this->size()*n);

        for (std::size_t i=0; i<n; i++) {
          result = result + *this;

          if (i < n - 1) {
            result = result + aggregator;
          }
        }

        return String{result};
      }

      String left_repeat(const String &value, std::size_t n, const String &aggregator = {}) const
      {
        return String(value).repeat(n, aggregator) + aggregator + *this;
      }

      String right_repeat(const String &value, std::size_t n, const String &aggregator = {}) const
      {
        return *this + aggregator + String(value).repeat(n, aggregator);
      }

      String align(align align, std::size_t length, const char fill = ' ') const
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

      String left(std::size_t length, const char fill = ' ') const
      {
        return align(align::left, length, fill);
      }

      String right(std::size_t length, const char fill = ' ') const
      {
        return align(align::right, length, fill);
      }

      String center(std::size_t length, const char fill = ' ') const
      {
        return align(align::center, length, fill);
      }

      String ellipses(std::size_t length, const String &value = String("...")) const
      {
        if (this->size() < length) {
          return *this;
        }

        return this->substr(0, length - value.size()) + value;
      }

      Iterator<std::vector<String>> split(const String &pattern = String("\\s+")) const
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

      bool is_blank() const
      {
        return match("[[:blank:]]*") or match("[[:space:]]*");
      }

      bool is_lower_case() const
      {
        return match("[[:lower:][:digit:][:graph:][:punct:][:space:]]*");
      }

      bool is_upper_case() const
      {
        return match("[[:upper:][:digit:][:graph:][:punct:][:space:]]*");
      }

      bool is_alpha() const
      {
        return match("[[:alpha:]]*");
      }

      bool is_number() const
      {
        return match("[[:digit:]]*.?[[:digit:]]*");
      }

      bool is_alpha_numeric() const
      {
        return match("[[:alnum:]]*");
      }

      String normalize() const
      {
        auto values = this->search("(\\w+)");

        std::string result;

        for (const auto &value : values) {
          result = result + value + " ";
        }

        return result.substr(0, result.size() - 1);
      }

      String encode_base64(const String &dictionary = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/") const
      {
        std::string out;
        int val = 0;
        int valb = -6;

        for (uint8_t c : *this) {
          val = (val << 8) + c;
          valb = valb + 8;

          while (valb >= 0) {
            out.push_back(dictionary[(val >> valb) & 0x3F]);

            valb = valb - 6;
          }
        }

        if (valb > -6) {
          out.push_back(dictionary[((val << 8) >> (valb + 8)) & 0x3F]);
        }

        while (out.size()%4) {
          out.push_back('=');
        }

        return out;
      }

      String decode_base64(const String &dictionary = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/") const
      {
        std::vector<int> T(256, -1);
        std::string out;

        for (int i=0; i<64; i++) {
          T[dictionary[i]] = i;
        }

        int val = 0;
        int valb = -8;

        for (uint8_t c : *this) {
          if (T[c] == -1) {
            break;
          }

          val = (val << 6) + T[c];
          valb = valb + 6;

          if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));

            valb = valb - 8;
          }
        }

        return out;
      }


      std::size_t levenshtein(const String &target, std::size_t insert_cost = 1, std::size_t delete_cost = 1, std::size_t replace_cost = 1) const
      {
        const String &source = *this;

        if (source.size() > target.size()) {
          return target.levenshtein(source, delete_cost, insert_cost, replace_cost);
        }

        const std::size_t min_size = source.size(), max_size = target.size();

        std::vector<std::size_t> lev_dist(min_size + 1);

        lev_dist[0] = 0;

        for (std::size_t i = 1; i <= min_size; i++) {
          lev_dist[i] = lev_dist[i - 1] + delete_cost;
        }

        for (std::size_t j = 1; j <= max_size; j++) {
          std::size_t previous_diagonal = lev_dist[0], previous_diagonal_save;

          lev_dist[0] += insert_cost;

          for (std::size_t i = 1; i <= min_size; i++) {
            previous_diagonal_save = lev_dist[i];

            if (source[i - 1] == target[j - 1]) {
              lev_dist[i] = previous_diagonal;
            } else {
              lev_dist[i] = std::min(std::min(lev_dist[i - 1] + delete_cost, lev_dist[i] + insert_cost), previous_diagonal + replace_cost);
            }

            previous_diagonal = previous_diagonal_save;
          }
        }

        return lev_dist[min_size];
      }

      double dice_coefficient(const String &target) const
      {
        std::set<String> string1_bigrams;
        std::set<String> string2_bigrams;

        // base case
        if (this->length() == 0 || target.length() == 0) {
          return 0.0;
        }

        for (std::size_t i=0; i<(this->length()-1); i++) {      // extract character bigrams from source
          string1_bigrams.insert(this->substr(i, 2));
        }

        for (std::size_t i=0; i<(target.length()-1); i++) {      // extract character bigrams from target
          string2_bigrams.insert(target.substr(i, 2));
        }

        std::size_t intersection = 0;

        // find the intersection between the two sets
        for (std::set<String>::iterator i=string2_bigrams.begin(); i!=string2_bigrams.end(); i++) {
          intersection += string1_bigrams.count((*i));
        }

        // calculate dice coefficient
        std::size_t total = string1_bigrams.size() + string2_bigrams.size();
        
        return (double)(intersection * 2)/(double)total;
      }

      /* -std=c++20 still not working
      template<typename ...Args>
      String format(Args &&...args)
      {
        return std::format(*this, std::forward<Args>(args)...);
      }
      */

  };

  template<typename Container>
    String join(const Container &values, String aggregate = String{","})
    {
      std::string result;

      for (const auto &value : values) {
        result = result + value + aggregate;
      }

      return result.substr(0, result.size() - aggregate.size());
    }

  String latin1_to_utf8(const String &str)
  {
    String out;

    for (String::const_iterator it = str.begin(); it != str.end(); ++it) {
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
