#pragma once

#include "jiterator.h"

#include <algorithm>
#include <optional>
#include <regex>
#include <initializer_list>
#include <set>
#include <codecvt>
#include <iomanip>
#include <sstream>

namespace jmixin {

  enum class align {
    left,
    center,
    right
  };

  class String : public std::string {

    public:
      String() = default;

      template<typename T>
      String(T &&str):
        std::string(str)
      {
      }

      Iterator<String> iterator() const
      {
        return Iterator<String>(*this);
      }

      std::wstring wide() const
      {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

        return converter.from_bytes(*this);
      }

      String hex()
      {
        static const char *hex_table[256] = {
          "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", 
          "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", 
          "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", 
          "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", 
          "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", 
          "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f", 
          "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f", 
          "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f", 
          "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f", 
          "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", 
          "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", 
          "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", 
          "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", 
          "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df", 
          "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef", 
          "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
        };

        String result;

        result.reserve(size()*2);

        for (auto ch : *this) {
          result = result + hex_table[static_cast<std::size_t>(ch)];
        }
        
        return result;
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
        return remove("^[[:space:]]*");
      }

      String right_trim()
      {
        return remove("[[:space:]]*$");
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

      Iterator<std::vector<Iterator<std::vector<String>>>> groups(const String &pattern, std::regex_constants::match_flag_type flags = std::regex_constants::match_default)
      {
        std::regex r(pattern, std::regex_constants::extended);
 
        Iterator<std::vector<Iterator<std::vector<String>>>> result;

        for(std::sregex_iterator i=std::sregex_iterator(this->begin(), this->end(), r, flags); i!=std::sregex_iterator(); i++) {
            std::smatch m = *i;

            Iterator<std::vector<String>> groupsResult;

            for(std::size_t index=0; index<m.size(); index++) {
                if (!m[index].str().empty()) {
                    groupsResult.push_back(m[index].str());
                }
            }

            result.push_back(groupsResult);
        }

        return result;
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
        String str = *this;

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

        typename String::const_iterator i = std::begin(*this);
        typename String::const_iterator j = std::cbegin(value);

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

        typename String::const_iterator i = std::begin(*this) + std::size(*this) - std::size(value);
        typename String::const_iterator j = std::cbegin(value);

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
        String result;

        result.reserve(this->size()*n);

        for (std::size_t i=0; i<n; i++) {
          result = result + *this;

          if (i < n - 1) {
            result = result + aggregator;
          }
        }

        return result;
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

      Iterator<std::vector<String>> split(const String &pattern = String("\\s")) const
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
        return replace(value, "");
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

        String result;

        for (const auto &value : values) {
          result = result + value + " ";
        }

        return result.substr(0, result.size() - 1);
      }

      String encode_base64(const String &dictionary = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/") const
      {
        String out;
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
        std::vector<int> v(256, -1);
        String out;

        for (int i=0; i<64; i++) {
          v[dictionary[i]] = i;
        }

        int val = 0;
        int valb = -8;

        for (uint8_t c : *this) {
          if (v[c] == -1) {
            break;
          }

          val = (val << 6) + v[c];
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
        for (typename std::set<String>::iterator i=string2_bigrams.begin(); i!=string2_bigrams.end(); i++) {
          intersection += string1_bigrams.count((*i));
        }

        // calculate dice coefficient
        std::size_t total = string1_bigrams.size() + string2_bigrams.size();
        
        return (double)(intersection * 2)/(double)total;
      }

      template<typename ...Args>
      String format(Args &&...args)
      {
        std::size_t size = snprintf(nullptr, 0, this->c_str(), args...) + 1;

        if (size <= 0) {
          throw std::runtime_error("Parser error");
        }

        std::unique_ptr<char[]> tmp(new char[size]);

        snprintf(tmp.get(), size, this->c_str(), args...);

        return std::string(tmp.get(), tmp.get() + size - 1);

        //  return std::format(*this, std::forward<Args>(args)...);
      }

      String asciify()
      {
        static struct {
          int id;
          const char *key;
          const char *value;
          int len;
        } mapping[766] = {
          { 0xc2a9, "©", "(c)", 3 },
          { 0xc2ab, "«", "<<", 2 },
          { 0xc2ae, "®", "(R)", 3 },
          { 0xc2bb, "»", ">>", 2 },
          { 0xc2bc, "¼", "1/4", 3 },
          { 0xc2bd, "½", "1/2", 3 },
          { 0xc2be, "¾", "3/4", 3 },
          { 0xc380, "À", "A", 1 },
          { 0xc381, "Á", "A", 1 },
          { 0xc382, "Â", "A", 1 },
          { 0xc383, "Ã", "A", 1 },
          { 0xc384, "Ä", "A", 1 },
          { 0xc385, "Å", "A", 1 },
          { 0xc386, "Æ", "AE", 2 },
          { 0xc387, "Ç", "C", 1 },
          { 0xc388, "È", "E", 1 },
          { 0xc389, "É", "E", 1 },
          { 0xc38a, "Ê", "E", 1 },
          { 0xc38b, "Ë", "E", 1 },
          { 0xc38c, "Ì", "I", 1 },
          { 0xc38d, "Í", "I", 1 },
          { 0xc38e, "Î", "I", 1 },
          { 0xc38f, "Ï", "I", 1 },
          { 0xc391, "Ñ", "N", 1 },
          { 0xc392, "Ò", "O", 1 },
          { 0xc393, "Ó", "O", 1 },
          { 0xc394, "Ô", "O", 1 },
          { 0xc395, "Õ", "O", 1 },
          { 0xc396, "Ö", "O", 1 },
          { 0xc398, "Ø", "O", 1 },
          { 0xc399, "Ù", "U", 1 },
          { 0xc39a, "Ú", "U", 1 },
          { 0xc39b, "Û", "U", 1 },
          { 0xc39c, "Ü", "U", 1 },
          { 0xc39d, "Ý", "Y", 1 },
          { 0xc3a0, "à", "a", 1 },
          { 0xc3a1, "á", "a", 1 },
          { 0xc3a2, "â", "a", 1 },
          { 0xc3a3, "ã", "a", 1 },
          { 0xc3a4, "ä", "a", 1 },
          { 0xc3a5, "å", "a", 1 },
          { 0xc3a6, "æ", "ae", 2 },
          { 0xc3a7, "ç", "c", 1 },
          { 0xc3a8, "è", "e", 1 },
          { 0xc3a9, "é", "e", 1 },
          { 0xc3aa, "ê", "e", 1 },
          { 0xc3ab, "ë", "e", 1 },
          { 0xc3ac, "ì", "i", 1 },
          { 0xc3ad, "í", "i", 1 },
          { 0xc3ae, "î", "i", 1 },
          { 0xc3af, "ï", "i", 1 },
          { 0xc3b1, "ñ", "n", 1 },
          { 0xc3b2, "ò", "o", 1 },
          { 0xc3b3, "ó", "o", 1 },
          { 0xc3b4, "ô", "o", 1 },
          { 0xc3b5, "õ", "o", 1 },
          { 0xc3b6, "ö", "o", 1 },
          { 0xc3b8, "ø", "o", 1 },
          { 0xc3b9, "ù", "u", 1 },
          { 0xc3ba, "ú", "u", 1 },
          { 0xc3bb, "û", "u", 1 },
          { 0xc3bc, "ü", "u", 1 },
          { 0xc3bd, "ý", "y", 1 },
          { 0xc3bf, "ÿ", "y", 1 },
          { 0xc480, "Ā", "A", 1 },
          { 0xc481, "ā", "a", 1 },
          { 0xc482, "Ă", "A", 1 },
          { 0xc483, "ă", "a", 1 },
          { 0xc484, "Ą", "A", 1 },
          { 0xc485, "ą", "a", 1 },
          { 0xc486, "Ć", "C", 1 },
          { 0xc487, "ć", "c", 1 },
          { 0xc488, "Ĉ", "C", 1 },
          { 0xc489, "ĉ", "c", 1 },
          { 0xc48a, "Ċ", "C", 1 },
          { 0xc48b, "ċ", "c", 1 },
          { 0xc48c, "Č", "C", 1 },
          { 0xc48d, "č", "c", 1 },
          { 0xc48e, "Ď", "D", 1 },
          { 0xc48f, "ď", "d", 1 },
          { 0xc490, "Đ", "D", 1 },
          { 0xc491, "đ", "d", 1 },
          { 0xc492, "Ē", "E", 1 },
          { 0xc493, "ē", "e", 1 },
          { 0xc494, "Ĕ", "E", 1 },
          { 0xc495, "ĕ", "e", 1 },
          { 0xc496, "Ė", "E", 1 },
          { 0xc497, "ė", "e", 1 },
          { 0xc498, "Ę", "E", 1 },
          { 0xc499, "ę", "e", 1 },
          { 0xc49a, "Ě", "E", 1 },
          { 0xc49b, "ě", "e", 1 },
          { 0xc49c, "Ĝ", "G", 1 },
          { 0xc49d, "ĝ", "g", 1 },
          { 0xc49e, "Ğ", "G", 1 },
          { 0xc49f, "ğ", "g", 1 },
          { 0xc4a0, "Ġ", "G", 1 },
          { 0xc4a1, "ġ", "g", 1 },
          { 0xc4a2, "Ģ", "G", 1 },
          { 0xc4a3, "ģ", "g", 1 },
          { 0xc4a4, "Ĥ", "H", 1 },
          { 0xc4a5, "ĥ", "h", 1 },
          { 0xc4a6, "Ħ", "H", 1 },
          { 0xc4a7, "ħ", "h", 1 },
          { 0xc4a8, "Ĩ", "I", 1 },
          { 0xc4a9, "ĩ", "i", 1 },
          { 0xc4aa, "Ī", "I", 1 },
          { 0xc4ab, "ī", "i", 1 },
          { 0xc4ac, "Ĭ", "I", 1 },
          { 0xc4ad, "ĭ", "i", 1 },
          { 0xc4ae, "Į", "I", 1 },
          { 0xc4af, "į", "i", 1 },
          { 0xc4b0, "İ", "I", 1 },
          { 0xc4b1, "ı", "i", 1 },
          { 0xc4b2, "Ĳ", "IJ", 2 },
          { 0xc4b3, "ĳ", "ij", 2 },
          { 0xc4b4, "Ĵ", "J", 1 },
          { 0xc4b5, "ĵ", "j", 1 },
          { 0xc4b6, "Ķ", "K", 1 },
          { 0xc4b7, "ķ", "k", 1 },
          { 0xc4b9, "Ĺ", "L", 1 },
          { 0xc4ba, "ĺ", "l", 1 },
          { 0xc4bb, "Ļ", "L", 1 },
          { 0xc4bc, "ļ", "l", 1 },
          { 0xc4bd, "Ľ", "L", 1 },
          { 0xc4be, "ľ", "l", 1 },
          { 0xc4bf, "Ŀ", "L", 1 },
          { 0xc580, "ŀ", "l", 1 },
          { 0xc581, "Ł", "L", 1 },
          { 0xc582, "ł", "l", 1 },
          { 0xc583, "Ń", "N", 1 },
          { 0xc584, "ń", "n", 1 },
          { 0xc585, "Ņ", "N", 1 },
          { 0xc586, "ņ", "n", 1 },
          { 0xc587, "Ň", "N", 1 },
          { 0xc588, "ň", "n", 1 },
          { 0xc589, "ŉ", "'n", 2 },
          { 0xc58a, "Ŋ", "N", 1 },
          { 0xc58b, "ŋ", "n", 1 },
          { 0xc58c, "Ō", "O", 1 },
          { 0xc58d, "ō", "o", 1 },
          { 0xc58e, "Ŏ", "O", 1 },
          { 0xc58f, "ŏ", "o", 1 },
          { 0xc590, "Ő", "O", 1 },
          { 0xc591, "ő", "o", 1 },
          { 0xc592, "Œ", "OE", 2 },
          { 0xc593, "œ", "oe", 2 },
          { 0xc594, "Ŕ", "R", 1 },
          { 0xc595, "ŕ", "r", 1 },
          { 0xc596, "Ŗ", "R", 1 },
          { 0xc597, "ŗ", "r", 1 },
          { 0xc598, "Ř", "R", 1 },
          { 0xc599, "ř", "r", 1 },
          { 0xc59a, "Ś", "S", 1 },
          { 0xc59b, "ś", "s", 1 },
          { 0xc59c, "Ŝ", "S", 1 },
          { 0xc59d, "ŝ", "s", 1 },
          { 0xc59e, "Ş", "S", 1 },
          { 0xc59f, "ş", "s", 1 },
          { 0xc5a0, "Š", "S", 1 },
          { 0xc5a1, "š", "s", 1 },
          { 0xc5a2, "Ţ", "T", 1 },
          { 0xc5a3, "ţ", "t", 1 },
          { 0xc5a4, "Ť", "T", 1 },
          { 0xc5a5, "ť", "t", 1 },
          { 0xc5a6, "Ŧ", "T", 1 },
          { 0xc5a7, "ŧ", "t", 1 },
          { 0xc5a8, "Ũ", "U", 1 },
          { 0xc5a9, "ũ", "u", 1 },
          { 0xc5aa, "Ū", "U", 1 },
          { 0xc5ab, "ū", "u", 1 },
          { 0xc5ac, "Ŭ", "U", 1 },
          { 0xc5ad, "ŭ", "u", 1 },
          { 0xc5ae, "Ů", "U", 1 },
          { 0xc5af, "ů", "u", 1 },
          { 0xc5b0, "Ű", "U", 1 },
          { 0xc5b1, "ű", "u", 1 },
          { 0xc5b2, "Ų", "U", 1 },
          { 0xc5b3, "ų", "u", 1 },
          { 0xc5b4, "Ŵ", "W", 1 },
          { 0xc5b5, "ŵ", "w", 1 },
          { 0xc5b6, "Ŷ", "Y", 1 },
          { 0xc5b7, "ŷ", "y", 1 },
          { 0xc5b8, "Ÿ", "Y", 1 },
          { 0xc5b9, "Ź", "Z", 1 },
          { 0xc5ba, "ź", "z", 1 },
          { 0xc5bb, "Ż", "Z", 1 },
          { 0xc5bc, "ż", "z", 1 },
          { 0xc5bd, "Ž", "Z", 1 },
          { 0xc5be, "ž", "z", 1 },
          { 0xc680, "ƀ", "b", 1 },
          { 0xc681, "Ɓ", "B", 1 },
          { 0xc682, "Ƃ", "B", 1 },
          { 0xc683, "ƃ", "b", 1 },
          { 0xc686, "Ɔ", "O", 1 },
          { 0xc687, "Ƈ", "C", 1 },
          { 0xc688, "ƈ", "c", 1 },
          { 0xc689, "Ɖ", "D", 1 },
          { 0xc68a, "Ɗ", "D", 1 },
          { 0xc68b, "Ƌ", "D", 1 },
          { 0xc68c, "ƌ", "d", 1 },
          { 0xc68e, "Ǝ", "E", 1 },
          { 0xc690, "Ɛ", "E", 1 },
          { 0xc691, "Ƒ", "F", 1 },
          { 0xc692, "ƒ", "f", 1 },
          { 0xc693, "Ɠ", "G", 1 },
          { 0xc697, "Ɨ", "I", 1 },
          { 0xc698, "Ƙ", "K", 1 },
          { 0xc699, "ƙ", "k", 1 },
          { 0xc69a, "ƚ", "l", 1 },
          { 0xc69c, "Ɯ", "M", 1 },
          { 0xc69d, "Ɲ", "N", 1 },
          { 0xc69e, "ƞ", "n", 1 },
          { 0xc69f, "Ɵ", "O", 1 },
          { 0xc6a0, "Ơ", "O", 1 },
          { 0xc6a1, "ơ", "o", 1 },
          { 0xc6a2, "Ƣ", "OI", 2 },
          { 0xc6a3, "ƣ", "oi", 2 },
          { 0xc6a4, "Ƥ", "P", 1 },
          { 0xc6a5, "ƥ", "p", 1 },
          { 0xc6ab, "ƫ", "t", 1 },
          { 0xc6ac, "Ƭ", "T", 1 },
          { 0xc6ad, "ƭ", "t", 1 },
          { 0xc6ae, "Ʈ", "T", 1 },
          { 0xc6af, "Ư", "U", 1 },
          { 0xc6b0, "ư", "u", 1 },
          { 0xc6b2, "Ʋ", "V", 1 },
          { 0xc6b3, "Ƴ", "Y", 1 },
          { 0xc6b4, "ƴ", "y", 1 },
          { 0xc6b5, "Ƶ", "Z", 1 },
          { 0xc6b6, "ƶ", "z", 1 },
          { 0xc6bb, "ƻ", "2", 1 },
          { 0xc784, "Ǆ", "DZ", 2 },
          { 0xc785, "ǅ", "Dz", 2 },
          { 0xc786, "ǆ", "dz", 2 },
          { 0xc787, "Ǉ", "LJ", 2 },
          { 0xc788, "ǈ", "Lj", 2 },
          { 0xc789, "ǉ", "lj", 2 },
          { 0xc78a, "Ǌ", "Nj", 2 },
          { 0xc78b, "ǋ", "Nj", 2 },
          { 0xc78c, "ǌ", "nj", 2 },
          { 0xc78d, "Ǎ", "A", 1 },
          { 0xc78e, "ǎ", "a", 1 },
          { 0xc78f, "Ǐ", "I", 1 },
          { 0xc790, "ǐ", "i", 1 },
          { 0xc791, "Ǒ", "O", 1 },
          { 0xc792, "ǒ", "o", 1 },
          { 0xc793, "Ǔ", "U", 1 },
          { 0xc794, "ǔ", "u", 1 },
          { 0xc795, "Ǖ", "U", 1 },
          { 0xc796, "ǖ", "u", 1 },
          { 0xc797, "Ǘ", "U", 1 },
          { 0xc798, "ǘ", "u", 1 },
          { 0xc799, "Ǚ", "U", 1 },
          { 0xc79a, "ǚ", "u", 1 },
          { 0xc79b, "Ǜ", "U", 1 },
          { 0xc79c, "ǜ", "u", 1 },
          { 0xc79d, "ǝ", "e", 1 },
          { 0xc79e, "Ǟ", "A", 1 },
          { 0xc79f, "ǟ", "a", 1 },
          { 0xc7a0, "Ǡ", "A", 1 },
          { 0xc7a1, "ǡ", "a", 1 },
          { 0xc7a2, "Ǣ", "AE", 2 },
          { 0xc7a3, "ǣ", "ae", 2 },
          { 0xc7a4, "Ǥ", "G", 1 },
          { 0xc7a5, "ǥ", "g", 1 },
          { 0xc7a6, "Ǧ", "G", 1 },
          { 0xc7a7, "ǧ", "g", 1 },
          { 0xc7a8, "Ǩ", "K", 1 },
          { 0xc7a9, "ǩ", "k", 1 },
          { 0xc7aa, "Ǫ", "O", 1 },
          { 0xc7ab, "ǫ", "o", 1 },
          { 0xc7ac, "Ǭ", "O", 1 },
          { 0xc7ad, "ǭ", "o", 1 },
          { 0xc7b0, "ǰ", "j", 1 },
          { 0xc7b1, "Ǳ", "DZ", 2 },
          { 0xc7b2, "ǲ", "Dz", 2 },
          { 0xc7b3, "ǳ", "dz", 2 },
          { 0xc7b4, "Ǵ", "G", 1 },
          { 0xc7b5, "ǵ", "g", 1 },
          { 0xc7b8, "Ǹ", "N", 1 },
          { 0xc7b9, "ǹ", "n", 1 },
          { 0xc7ba, "Ǻ", "A", 1 },
          { 0xc7bb, "ǻ", "a", 1 },
          { 0xc7bc, "Ǽ", "AE", 2 },
          { 0xc7bd, "ǽ", "ae", 2 },
          { 0xc7be, "Ǿ", "O", 1 },
          { 0xc7bf, "ǿ", "o", 1 },
          { 0xc880, "Ȁ", "A", 1 },
          { 0xc881, "ȁ", "a", 1 },
          { 0xc882, "Ȃ", "A", 1 },
          { 0xc883, "ȃ", "a", 1 },
          { 0xc884, "Ȅ", "E", 1 },
          { 0xc885, "ȅ", "e", 1 },
          { 0xc886, "Ȇ", "E", 1 },
          { 0xc887, "ȇ", "e", 1 },
          { 0xc888, "Ȉ", "I", 1 },
          { 0xc889, "ȉ", "i", 1 },
          { 0xc88a, "Ȋ", "I", 1 },
          { 0xc88b, "ȋ", "i", 1 },
          { 0xc88c, "Ȍ", "O", 1 },
          { 0xc88d, "ȍ", "o", 1 },
          { 0xc88e, "Ȏ", "O", 1 },
          { 0xc88f, "ȏ", "o", 1 },
          { 0xc890, "Ȑ", "R", 1 },
          { 0xc891, "ȑ", "r", 1 },
          { 0xc892, "Ȓ", "R", 1 },
          { 0xc893, "ȓ", "r", 1 },
          { 0xc894, "Ȕ", "U", 1 },
          { 0xc895, "ȕ", "u", 1 },
          { 0xc896, "Ȗ", "U", 1 },
          { 0xc897, "ȗ", "u", 1 },
          { 0xc898, "Ș", "S", 1 },
          { 0xc899, "ș", "s", 1 },
          { 0xc89a, "Ț", "T", 1 },
          { 0xc89b, "ț", "t", 1 },
          { 0xc89e, "Ȟ", "H", 1 },
          { 0xc89f, "ȟ", "h", 1 },
          { 0xc8a0, "Ƞ", "N", 1 },
          { 0xc8a1, "ȡ", "d", 1 },
          { 0xc8a4, "Ȥ", "Z", 1 },
          { 0xc8a5, "ȥ", "z", 1 },
          { 0xc8a6, "Ȧ", "A", 1 },
          { 0xc8a7, "ȧ", "a", 1 },
          { 0xc8a8, "Ȩ", "E", 1 },
          { 0xc8a9, "ȩ", "e", 1 },
          { 0xc8aa, "Ȫ", "O", 1 },
          { 0xc8ab, "ȫ", "o", 1 },
          { 0xc8ac, "Ȭ", "O", 1 },
          { 0xc8ad, "ȭ", "o", 1 },
          { 0xc8ae, "Ȯ", "O", 1 },
          { 0xc8af, "ȯ", "o", 1 },
          { 0xc8b0, "Ȱ", "O", 1 },
          { 0xc8b1, "ȱ", "o", 1 },
          { 0xc8b2, "Ȳ", "Y", 1 },
          { 0xc8b3, "ȳ", "y", 1 },
          { 0xc8b4, "ȴ", "l", 1 },
          { 0xc8b5, "ȵ", "n", 1 },
          { 0xc8b6, "ȶ", "t", 1 },
          { 0xc8b7, "ȷ", "j", 1 },
          { 0xc8b8, "ȸ", "db", 2 },
          { 0xc8b9, "ȹ", "qp", 2 },
          { 0xc8ba, "Ⱥ", "A", 1 },
          { 0xc8bb, "Ȼ", "C", 1 },
          { 0xc8bc, "ȼ", "c", 1 },
          { 0xc8bd, "Ƚ", "L", 1 },
          { 0xc8be, "Ⱦ", "T", 1 },
          { 0xc8bf, "ȿ", "s", 1 },
          { 0xc980, "ɀ", "z", 1 },
          { 0xc983, "Ƀ", "B", 1 },
          { 0xc984, "Ʉ", "U", 1 },
          { 0xc985, "Ʌ", "V", 1 },
          { 0xc986, "Ɇ", "E", 1 },
          { 0xc987, "ɇ", "e", 1 },
          { 0xc988, "Ɉ", "J", 1 },
          { 0xc989, "ɉ", "j", 1 },
          { 0xc98a, "Ɋ", "Q", 1 },
          { 0xc98b, "ɋ", "q", 1 },
          { 0xc98c, "Ɍ", "R", 1 },
          { 0xc98d, "ɍ", "r", 1 },
          { 0xc98e, "Ɏ", "Y", 1 },
          { 0xc98f, "ɏ", "y", 1 },
          { 0xc990, "ɐ", "a", 1 },
          { 0xc993, "ɓ", "b", 1 },
          { 0xc994, "ɔ", "o", 1 },
          { 0xc995, "ɕ", "c", 1 },
          { 0xc996, "ɖ", "d", 1 },
          { 0xc997, "ɗ", "d", 1 },
          { 0xc998, "ɘ", "e", 1 },
          { 0xc99b, "ɛ", "e", 1 },
          { 0xc99c, "ɜ", "e", 1 },
          { 0xc99d, "ɝ", "e", 1 },
          { 0xc99e, "ɞ", "e", 1 },
          { 0xc99f, "ɟ", "j", 1 },
          { 0xc9a0, "ɠ", "g", 1 },
          { 0xc9a1, "ɡ", "g", 1 },
          { 0xc9a2, "ɢ", "G", 1 },
          { 0xc9a5, "ɥ", "h", 1 },
          { 0xc9a6, "ɦ", "h", 1 },
          { 0xc9a8, "ɨ", "i", 1 },
          { 0xc9aa, "ɪ", "I", 1 },
          { 0xc9ab, "ɫ", "l", 1 },
          { 0xc9ac, "ɬ", "l", 1 },
          { 0xc9ad, "ɭ", "l", 1 },
          { 0xc9af, "ɯ", "m", 1 },
          { 0xc9b0, "ɰ", "m", 1 },
          { 0xc9b1, "ɱ", "m", 1 },
          { 0xc9b2, "ɲ", "n", 1 },
          { 0xc9b3, "ɳ", "n", 1 },
          { 0xc9b4, "ɴ", "N", 1 },
          { 0xc9b5, "ɵ", "o", 1 },
          { 0xc9b6, "ɶ", "OE", 2 },
          { 0xc9b9, "ɹ", "r", 1 },
          { 0xc9ba, "ɺ", "r", 1 },
          { 0xc9bb, "ɻ", "r", 1 },
          { 0xc9bc, "ɼ", "r", 1 },
          { 0xc9bd, "ɽ", "r", 1 },
          { 0xc9be, "ɾ", "r", 1 },
          { 0xc9bf, "ɿ", "r", 1 },
          { 0xca80, "ʀ", "R", 1 },
          { 0xca81, "ʁ", "R", 1 },
          { 0xca82, "ʂ", "s", 1 },
          { 0xca87, "ʇ", "t", 1 },
          { 0xca88, "ʈ", "t", 1 },
          { 0xca89, "ʉ", "u", 1 },
          { 0xca8b, "ʋ", "v", 1 },
          { 0xca8c, "ʌ", "v", 1 },
          { 0xca8d, "ʍ", "w", 1 },
          { 0xca8e, "ʎ", "y", 1 },
          { 0xca8f, "ʏ", "Y", 1 },
          { 0xca90, "ʐ", "z", 1 },
          { 0xca91, "ʑ", "z", 1 },
          { 0xca97, "ʗ", "C", 1 },
          { 0xca99, "ʙ", "B", 1 },
          { 0xca9a, "ʚ", "e", 1 },
          { 0xca9b, "ʛ", "G", 1 },
          { 0xca9c, "ʜ", "H", 1 },
          { 0xca9d, "ʝ", "j", 1 },
          { 0xca9e, "ʞ", "k", 1 },
          { 0xca9f, "ʟ", "L", 1 },
          { 0xcaa0, "ʠ", "q", 1 },
          { 0xcaa3, "ʣ", "dz", 2 },
          { 0xcaa5, "ʥ", "dz", 2 },
          { 0xcaa6, "ʦ", "ts", 2 },
          { 0xcaa8, "ʨ", "tc", 2 },
          { 0xcaaa, "ʪ", "ls", 2 },
          { 0xcaab, "ʫ", "lz", 2 },
          { 0xcaae, "ʮ", "h", 1 },
          { 0xcaaf, "ʯ", "h", 1 },
          { 0xe1b880, "Ḁ", "A", 1 },
          { 0xe1b881, "ḁ", "a", 1 },
          { 0xe1b882, "Ḃ", "B", 1 },
          { 0xe1b883, "ḃ", "b", 1 },
          { 0xe1b884, "Ḅ", "B", 1 },
          { 0xe1b885, "ḅ", "b", 1 },
          { 0xe1b886, "Ḇ", "B", 1 },
          { 0xe1b887, "ḇ", "b", 1 },
          { 0xe1b888, "Ḉ", "C", 1 },
          { 0xe1b889, "ḉ", "c", 1 },
          { 0xe1b88a, "Ḋ", "D", 1 },
          { 0xe1b88b, "ḋ", "d", 1 },
          { 0xe1b88c, "Ḍ", "D", 1 },
          { 0xe1b88d, "ḍ", "d", 1 },
          { 0xe1b88e, "Ḏ", "D", 1 },
          { 0xe1b88f, "ḏ", "d", 1 },
          { 0xe1b890, "Ḑ", "D", 1 },
          { 0xe1b891, "ḑ", "d", 1 },
          { 0xe1b892, "Ḓ", "D", 1 },
          { 0xe1b893, "ḓ", "d", 1 },
          { 0xe1b894, "Ḕ", "E", 1 },
          { 0xe1b895, "ḕ", "e", 1 },
          { 0xe1b896, "Ḗ", "E", 1 },
          { 0xe1b897, "ḗ", "e", 1 },
          { 0xe1b898, "Ḙ", "E", 1 },
          { 0xe1b899, "ḙ", "e", 1 },
          { 0xe1b89a, "Ḛ", "E", 1 },
          { 0xe1b89b, "ḛ", "e", 1 },
          { 0xe1b89c, "Ḝ", "E", 1 },
          { 0xe1b89d, "ḝ", "e", 1 },
          { 0xe1b89e, "Ḟ", "F", 1 },
          { 0xe1b89f, "ḟ", "f", 1 },
          { 0xe1b8a0, "Ḡ", "G", 1 },
          { 0xe1b8a1, "ḡ", "g", 1 },
          { 0xe1b8a2, "Ḣ", "H", 1 },
          { 0xe1b8a3, "ḣ", "h", 1 },
          { 0xe1b8a4, "Ḥ", "H", 1 },
          { 0xe1b8a5, "ḥ", "h", 1 },
          { 0xe1b8a6, "Ḧ", "H", 1 },
          { 0xe1b8a7, "ḧ", "h", 1 },
          { 0xe1b8a8, "Ḩ", "H", 1 },
          { 0xe1b8a9, "ḩ", "h", 1 },
          { 0xe1b8aa, "Ḫ", "H", 1 },
          { 0xe1b8ab, "ḫ", "h", 1 },
          { 0xe1b8ac, "Ḭ", "I", 1 },
          { 0xe1b8ad, "ḭ", "i", 1 },
          { 0xe1b8ae, "Ḯ", "I", 1 },
          { 0xe1b8af, "ḯ", "i", 1 },
          { 0xe1b8b0, "Ḱ", "K", 1 },
          { 0xe1b8b1, "ḱ", "k", 1 },
          { 0xe1b8b2, "Ḳ", "K", 1 },
          { 0xe1b8b3, "ḳ", "k", 1 },
          { 0xe1b8b4, "Ḵ", "K", 1 },
          { 0xe1b8b5, "ḵ", "k", 1 },
          { 0xe1b8b6, "Ḷ", "L", 1 },
          { 0xe1b8b7, "ḷ", "l", 1 },
          { 0xe1b8b8, "Ḹ", "L", 1 },
          { 0xe1b8b9, "ḹ", "l", 1 },
          { 0xe1b8ba, "Ḻ", "L", 1 },
          { 0xe1b8bb, "ḻ", "l", 1 },
          { 0xe1b8bc, "Ḽ", "L", 1 },
          { 0xe1b8bd, "ḽ", "l", 1 },
          { 0xe1b8be, "Ḿ", "M", 1 },
          { 0xe1b8bf, "ḿ", "m", 1 },
          { 0xe1b980, "Ṁ", "M", 1 },
          { 0xe1b981, "ṁ", "m", 1 },
          { 0xe1b982, "Ṃ", "M", 1 },
          { 0xe1b983, "ṃ", "m", 1 },
          { 0xe1b984, "Ṅ", "N", 1 },
          { 0xe1b985, "ṅ", "n", 1 },
          { 0xe1b986, "Ṇ", "N", 1 },
          { 0xe1b987, "ṇ", "n", 1 },
          { 0xe1b988, "Ṉ", "N", 1 },
          { 0xe1b989, "ṉ", "n", 1 },
          { 0xe1b98a, "Ṋ", "N", 1 },
          { 0xe1b98b, "ṋ", "n", 1 },
          { 0xe1b98c, "Ṍ", "O", 1 },
          { 0xe1b98d, "ṍ", "o", 1 },
          { 0xe1b98e, "Ṏ", "O", 1 },
          { 0xe1b98f, "ṏ", "o", 1 },
          { 0xe1b990, "Ṑ", "O", 1 },
          { 0xe1b991, "ṑ", "o", 1 },
          { 0xe1b992, "Ṓ", "O", 1 },
          { 0xe1b993, "ṓ", "o", 1 },
          { 0xe1b994, "Ṕ", "P", 1 },
          { 0xe1b995, "ṕ", "p", 1 },
          { 0xe1b996, "Ṗ", "P", 1 },
          { 0xe1b997, "ṗ", "p", 1 },
          { 0xe1b998, "Ṙ", "R", 1 },
          { 0xe1b999, "ṙ", "r", 1 },
          { 0xe1b99a, "Ṛ", "R", 1 },
          { 0xe1b99b, "ṛ", "r", 1 },
          { 0xe1b99c, "Ṝ", "R", 1 },
          { 0xe1b99d, "ṝ", "r", 1 },
          { 0xe1b99e, "Ṟ", "R", 1 },
          { 0xe1b99f, "ṟ", "r", 1 },
          { 0xe1b9a0, "Ṡ", "S", 1 },
          { 0xe1b9a1, "ṡ", "s", 1 },
          { 0xe1b9a2, "Ṣ", "S", 1 },
          { 0xe1b9a3, "ṣ", "s", 1 },
          { 0xe1b9a4, "Ṥ", "S", 1 },
          { 0xe1b9a5, "ṥ", "s", 1 },
          { 0xe1b9a6, "Ṧ", "S", 1 },
          { 0xe1b9a7, "ṧ", "s", 1 },
          { 0xe1b9a8, "Ṩ", "S", 1 },
          { 0xe1b9a9, "ṩ", "s", 1 },
          { 0xe1b9aa, "Ṫ", "T", 1 },
          { 0xe1b9ab, "ṫ", "t", 1 },
          { 0xe1b9ac, "Ṭ", "T", 1 },
          { 0xe1b9ad, "ṭ", "t", 1 },
          { 0xe1b9ae, "Ṯ", "T", 1 },
          { 0xe1b9af, "ṯ", "t", 1 },
          { 0xe1b9b0, "Ṱ", "T", 1 },
          { 0xe1b9b1, "ṱ", "t", 1 },
          { 0xe1b9b2, "Ṳ", "U", 1 },
          { 0xe1b9b3, "ṳ", "u", 1 },
          { 0xe1b9b4, "Ṵ", "U", 1 },
          { 0xe1b9b5, "ṵ", "u", 1 },
          { 0xe1b9b6, "Ṷ", "U", 1 },
          { 0xe1b9b7, "ṷ", "u", 1 },
          { 0xe1b9b8, "Ṹ", "U", 1 },
          { 0xe1b9b9, "ṹ", "u", 1 },
          { 0xe1b9ba, "Ṻ", "U", 1 },
          { 0xe1b9bb, "ṻ", "u", 1 },
          { 0xe1b9bc, "Ṽ", "V", 1 },
          { 0xe1b9bd, "ṽ", "v", 1 },
          { 0xe1b9be, "Ṿ", "V", 1 },
          { 0xe1b9bf, "ṿ", "v", 1 },
          { 0xe1ba80, "Ẁ", "W", 1 },
          { 0xe1ba81, "ẁ", "w", 1 },
          { 0xe1ba82, "Ẃ", "W", 1 },
          { 0xe1ba83, "ẃ", "w", 1 },
          { 0xe1ba84, "Ẅ", "W", 1 },
          { 0xe1ba85, "ẅ", "w", 1 },
          { 0xe1ba86, "Ẇ", "W", 1 },
          { 0xe1ba87, "ẇ", "w", 1 },
          { 0xe1ba88, "Ẉ", "W", 1 },
          { 0xe1ba89, "ẉ", "w", 1 },
          { 0xe1ba8a, "Ẋ", "X", 1 },
          { 0xe1ba8b, "ẋ", "x", 1 },
          { 0xe1ba8c, "Ẍ", "X", 1 },
          { 0xe1ba8d, "ẍ", "x", 1 },
          { 0xe1ba8e, "Ẏ", "Y", 1 },
          { 0xe1ba8f, "ẏ", "y", 1 },
          { 0xe1ba90, "Ẑ", "Z", 1 },
          { 0xe1ba91, "ẑ", "z", 1 },
          { 0xe1ba92, "Ẓ", "Z", 1 },
          { 0xe1ba93, "ẓ", "z", 1 },
          { 0xe1ba94, "Ẕ", "Z", 1 },
          { 0xe1ba95, "ẕ", "z", 1 },
          { 0xe1ba96, "ẖ", "h", 1 },
          { 0xe1ba97, "ẗ", "t", 1 },
          { 0xe1ba98, "ẘ", "w", 1 },
          { 0xe1ba99, "ẙ", "y", 1 },
          { 0xe1ba9a, "ẚ", "a", 1 },
          { 0xe1baa0, "Ạ", "A", 1 },
          { 0xe1baa1, "ạ", "a", 1 },
          { 0xe1baa2, "Ả", "A", 1 },
          { 0xe1baa3, "ả", "a", 1 },
          { 0xe1baa4, "Ấ", "A", 1 },
          { 0xe1baa5, "ấ", "a", 1 },
          { 0xe1baa6, "Ầ", "A", 1 },
          { 0xe1baa7, "ầ", "a", 1 },
          { 0xe1baa8, "Ẩ", "A", 1 },
          { 0xe1baa9, "ẩ", "a", 1 },
          { 0xe1baaa, "Ẫ", "A", 1 },
          { 0xe1baab, "ẫ", "a", 1 },
          { 0xe1baac, "Ậ", "A", 1 },
          { 0xe1baad, "ậ", "a", 1 },
          { 0xe1baae, "Ắ", "A", 1 },
          { 0xe1baaf, "ắ", "a", 1 },
          { 0xe1bab0, "Ằ", "A", 1 },
          { 0xe1bab1, "ằ", "a", 1 },
          { 0xe1bab2, "Ẳ", "A", 1 },
          { 0xe1bab3, "ẳ", "a", 1 },
          { 0xe1bab4, "Ẵ", "A", 1 },
          { 0xe1bab5, "ẵ", "a", 1 },
          { 0xe1bab6, "Ặ", "A", 1 },
          { 0xe1bab7, "ặ", "a", 1 },
          { 0xe1bab8, "Ẹ", "E", 1 },
          { 0xe1bab9, "ẹ", "e", 1 },
          { 0xe1baba, "Ẻ", "E", 1 },
          { 0xe1babb, "ẻ", "e", 1 },
          { 0xe1babc, "Ẽ", "E", 1 },
          { 0xe1babd, "ẽ", "e", 1 },
          { 0xe1babe, "Ế", "E", 1 },
          { 0xe1babf, "ế", "e", 1 },
          { 0xe1bb80, "Ề", "E", 1 },
          { 0xe1bb81, "ề", "e", 1 },
          { 0xe1bb82, "Ể", "E", 1 },
          { 0xe1bb83, "ể", "e", 1 },
          { 0xe1bb84, "Ễ", "E", 1 },
          { 0xe1bb85, "ễ", "e", 1 },
          { 0xe1bb86, "Ệ", "E", 1 },
          { 0xe1bb87, "ệ", "e", 1 },
          { 0xe1bb88, "Ỉ", "I", 1 },
          { 0xe1bb89, "ỉ", "i", 1 },
          { 0xe1bb8a, "Ị", "I", 1 },
          { 0xe1bb8b, "ị", "i", 1 },
          { 0xe1bb8c, "Ọ", "O", 1 },
          { 0xe1bb8d, "ọ", "o", 1 },
          { 0xe1bb8e, "Ỏ", "O", 1 },
          { 0xe1bb8f, "ỏ", "o", 1 },
          { 0xe1bb90, "Ố", "O", 1 },
          { 0xe1bb91, "ố", "o", 1 },
          { 0xe1bb92, "Ồ", "O", 1 },
          { 0xe1bb93, "ồ", "o", 1 },
          { 0xe1bb94, "Ổ", "O", 1 },
          { 0xe1bb95, "ổ", "o", 1 },
          { 0xe1bb96, "Ỗ", "O", 1 },
          { 0xe1bb97, "ỗ", "o", 1 },
          { 0xe1bb98, "Ộ", "O", 1 },
          { 0xe1bb99, "ộ", "o", 1 },
          { 0xe1bb9a, "Ớ", "O", 1 },
          { 0xe1bb9b, "ớ", "o", 1 },
          { 0xe1bb9c, "Ờ", "O", 1 },
          { 0xe1bb9d, "ờ", "o", 1 },
          { 0xe1bb9e, "Ở", "O", 1 },
          { 0xe1bb9f, "ở", "o", 1 },
          { 0xe1bba0, "Ỡ", "O", 1 },
          { 0xe1bba1, "ỡ", "o", 1 },
          { 0xe1bba2, "Ợ", "O", 1 },
          { 0xe1bba3, "ợ", "o", 1 },
          { 0xe1bba4, "Ụ", "U", 1 },
          { 0xe1bba5, "ụ", "u", 1 },
          { 0xe1bba6, "Ủ", "U", 1 },
          { 0xe1bba7, "ủ", "u", 1 },
          { 0xe1bba8, "Ứ", "U", 1 },
          { 0xe1bba9, "ứ", "u", 1 },
          { 0xe1bbaa, "Ừ", "U", 1 },
          { 0xe1bbab, "ừ", "u", 1 },
          { 0xe1bbac, "Ử", "U", 1 },
          { 0xe1bbad, "ử", "u", 1 },
          { 0xe1bbae, "Ữ", "U", 1 },
          { 0xe1bbaf, "ữ", "u", 1 },
          { 0xe1bbb0, "Ự", "U", 1 },
          { 0xe1bbb1, "ự", "u", 1 },
          { 0xe1bbb2, "Ỳ", "Y", 1 },
          { 0xe1bbb3, "ỳ", "y", 1 },
          { 0xe1bbb4, "Ỵ", "Y", 1 },
          { 0xe1bbb5, "ỵ", "y", 1 },
          { 0xe1bbb6, "Ỷ", "Y", 1 },
          { 0xe1bbb7, "ỷ", "y", 1 },
          { 0xe1bbb8, "Ỹ", "Y", 1 },
          { 0xe1bbb9, "ỹ", "y", 1 },
          { 0xe28080, " ", " ", 1 },
          { 0xe28081, " ", " ", 1 },
          { 0xe28082, " ", " ", 1 },
          { 0xe28083, " ", " ", 1 },
          { 0xe28084, " ", " ", 1 },
          { 0xe28085, " ", " ", 1 },
          { 0xe28086, " ", " ", 1 },
          { 0xe28087, " ", " ", 1 },
          { 0xe28088, " ", " ", 1 },
          { 0xe28089, " ", " ", 1 },
          { 0xe2808a, " ", " ", 1 },
          { 0xe2808b, "​", "", 0 },
          { 0xe2808c, "‌", "", 0 },
          { 0xe2808d, "‍", "", 0 },
          { 0xe28090, "‐", "-", 1 },
          { 0xe28091, "‑", "-", 1 },
          { 0xe28092, "‒", "-", 1 },
          { 0xe28093, "–", "-", 1 },
          { 0xe28094, "—", "-", 1 },
          { 0xe28095, "―", "-", 1 },
          { 0xe28096, "‖", "||", 2 },
          { 0xe28098, "‘", "'", 1 },
          { 0xe28099, "’", "'", 1 },
          { 0xe2809a, "‚", ",", 1 },
          { 0xe2809b, "‛", "'", 1 },
          { 0xe2809c, "“", "\"", 1 },
          { 0xe2809d, "”", "\"", 1 },
          { 0xe2809f, "‟", "\"", 1 },
          { 0xe280a4, "․", ".", 1 },
          { 0xe280a5, "‥", "..", 2 },
          { 0xe280a6, "…", "...", 3 },
          { 0xe280af, " ", " ", 1 },
          { 0xe280b2, "′", "'", 1 },
          { 0xe280b3, "″", "\"", 1 },
          { 0xe280b4, "‴", "'\"", 2 },
          { 0xe280b5, "‵", "'", 1 },
          { 0xe280b6, "‶", "\"", 1 },
          { 0xe280b7, "‷", "\"'", 2 },
          { 0xe280b9, "‹", "<", 1 },
          { 0xe280ba, "›", ">", 1 },
          { 0xe280bc, "‼", "!!", 2 },
          { 0xe280bd, "‽", "?!", 2 },
          { 0xe28184, "⁄", "/", 1 },
          { 0xe28187, "⁇", "?/", 2 },
          { 0xe28188, "⁈", "?!", 2 },
          { 0xe28189, "⁉", "!?", 2 },
          { 0xe284a0, "℠", "SM", 2 },
          { 0xe284a2, "™", "TM", 2 },
          { 0xe28593, "⅓", "1/3", 3 },
          { 0xe28594, "⅔", "2/3", 3 },
          { 0xe28595, "⅕", "1/5", 3 },
          { 0xe28596, "⅖", "2/5", 3 },
          { 0xe28597, "⅗", "3/5", 3 },
          { 0xe28598, "⅘", "4/5", 3 },
          { 0xe28599, "⅙", "1/6", 3 },
          { 0xe2859a, "⅚", "5/6", 3 },
          { 0xe2859b, "⅛", "1/8", 3 },
          { 0xe2859c, "⅜", "3/8", 3 },
          { 0xe2859d, "⅝", "5/8", 3 },
          { 0xe2859e, "⅞", "7/8", 3 },
          { 0xe285a0, "Ⅰ", "I", 1 },
          { 0xe285a1, "Ⅱ", "II", 2 },
          { 0xe285a2, "Ⅲ", "III", 3 },
          { 0xe285a3, "Ⅳ", "IV", 2 },
          { 0xe285a4, "Ⅴ", "V", 1 },
          { 0xe285a5, "Ⅵ", "Vi", 2 },
          { 0xe285a6, "Ⅶ", "VII", 3 },
          { 0xe285a7, "Ⅷ", "VIII", 4 },
          { 0xe285a8, "Ⅸ", "IX", 2 },
          { 0xe285a9, "Ⅹ", "X", 1 },
          { 0xe285aa, "Ⅺ", "XI", 2 },
          { 0xe285ab, "Ⅻ", "XII", 3 },
          { 0xe285ac, "Ⅼ", "L", 1 },
          { 0xe285ad, "Ⅽ", "C", 1 },
          { 0xe285ae, "Ⅾ", "D", 1 },
          { 0xe285af, "Ⅿ", "M", 1 },
          { 0xe285b0, "ⅰ", "i", 1 },
          { 0xe285b1, "ⅱ", "ii", 2 },
          { 0xe285b2, "ⅲ", "iii", 3 },
          { 0xe285b3, "ⅳ", "iv", 2 },
          { 0xe285b4, "ⅴ", "v", 1 },
          { 0xe285b5, "ⅵ", "vi", 2 },
          { 0xe285b6, "ⅶ", "vii", 3 },
          { 0xe285b7, "ⅷ", "viii", 4 },
          { 0xe285b8, "ⅸ", "ix", 2 },
          { 0xe285b9, "ⅹ", "x", 1 },
          { 0xe285ba, "ⅺ", "xi", 2 },
          { 0xe285bb, "ⅻ", "xii", 3 },
          { 0xe285bc, "ⅼ", "l", 1 },
          { 0xe285bd, "ⅽ", "c", 1 },
          { 0xe285be, "ⅾ", "d", 1 },
          { 0xe285bf, "ⅿ", "m", 1 }
        };

        auto search = [](int id) -> int {
          int high = sizeof(mapping) / sizeof(mapping[0]) - 1;
          int low = 0;

          while (low <= high) {
            auto mid = (low + high) / 2;

            if (mapping[mid].id == id) {
              return mid;
            } else if (mapping[mid].id < id) {
              low = mid + 1;
            } else {
              high = mid - 1;
            }
          }

          return -1;
        };

        const char *utf8 = this->c_str();
        std::size_t len = this->size();
        char *ascii = new char[len * 4 + 1];
        std::size_t in = 0;
        int out = 0;
        int key = 0;
        int id = 0;
        char *idp = (char*)&id;
        
        ascii[0] = 0;
        
        while (in < len) {
          if (in + 3 <= len) {
            idp[0] = utf8[in + 2];
            idp[1] = utf8[in + 1];
            idp[2] = utf8[in + 0];
            idp[3] = 0;

            key = search(id);
            
            if (key != -1) {
              strcat(ascii, mapping[key].value);
              in += 3;
              out += mapping[key].len;

              continue;
            }
          }

          if (in + 2 <= len) {
            idp[0] = utf8[in + 1];
            idp[1] = utf8[in + 0];
            idp[2] = 0;
            idp[3] = 0;
            key = search(id);
            
            if (key != -1) {
              strcat(ascii, mapping[key].value);
              in += 2;
              out += mapping[key].len;

              continue;
            }
          }

          if ((utf8[in] & 0xff) <= 0x7f) {
            ascii[out++] = utf8[in++];
            ascii[out] = 0;
          } else {
            in++;
          }
        }

        return std::string(ascii);
      }

      String encode_url(bool upper_case = false)
      {
        static const int kUnreservedChar[] = {
        //0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, // 2
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
          0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, // 5
          0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, // 7
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // F
        };

        std::ostringstream o;

        for (std::string::const_iterator it = this->begin(); it != this->end(); ++it) {
          if (kUnreservedChar[static_cast<std::size_t>(*it)]) {
            o << *it;
          } else {
            o << '%' << std::setfill('0') << std::hex;
            
            if (upper_case) {
              o << std::uppercase;
            }

            o << (int)*it;
          }
        }

        return o.str();
      }

      String decode_url()
      {
        static const int kHexToNum[] = {
        // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 1
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 2
           0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 3
          -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 4
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 5
          -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 6
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 7
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 8
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 9
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // A
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // B
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // C
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // D
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // E
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // F
        };

        std::stringstream o;
        std::string::const_iterator it = this->begin();
        char tmp, encoded_char;

        while (it != this->end()) {
          if (*it == '%') {
            if (++it == this->end()) {
              throw std::runtime_error("Invalid encoded url");
            }

            tmp = static_cast<char>(kHexToNum[static_cast<std::size_t>(*it)]);
            
            if (tmp < 0) {
              throw std::runtime_error("Invalid encoded url");
            }

            encoded_char = (16 * tmp);

            if (++it == this->end()) {
              throw std::runtime_error("Invalid encoded url");
            }

            tmp = kHexToNum[static_cast<std::size_t>(*it)];
            
            if (tmp < 0) {
              throw std::runtime_error("Invalid encoded url");
            }

            encoded_char += tmp;

            it++;

            o << encoded_char;
          } else {
            o << *it++;
          }
        }

        return o.str();
      }

  };

  template<typename Container>
    String join(const Container &values, std::string aggregate = std::string{","})
    {
      String result;

      for (const auto &value : values) {
        result = result + value + aggregate;
      }

      return result.substr(0, result.size() - aggregate.size());
    }

  String from_hex(const std::string &str);
  
  String latin1_to_utf8(const std::string &str);

}

