#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>
#include <numeric>
#include <random>

namespace jmixin {

  template<typename Container>
    class Iterator : public Container {

      public:
        Iterator(const Container &container):
          Container(container)
        {
        }

        Iterator<Container> copy()
        {
          Container result;

          std::copy(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

        template<typename Predicate>
          std::size_t count(Predicate predicate)
          {
            return std::count_if(std::begin(*this), std::end(*this), predicate);
          }

        template<typename Predicate>
          Iterator<Container> filter(Predicate predicate)
          {
            Container result;

            std::copy_if(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), predicate);

            return Iterator<Container>(result);
          }

        Iterator<Container> step(std::size_t n)
        {
          Container result;
          std::size_t i {0};

          std::copy_if(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), 
              [&i, n](auto const &item) {
              if ((i++ % n) == 0) {
              return true;
              }

              return false;
              });

          return Iterator<Container>(result);
        }

        template<typename Initial>
        typename Container::value_type sum(Initial initial = Initial())
        {
          Container result;

          return std::accumulate(std::begin(*this), std::end(*this), initial);
        }

        template<typename Initial>
        typename Container::value_type product(Initial initial = Initial())
        {
          Container result;

          return std::accumulate(std::begin(*this), std::end(*this), initial, std::multiplies<typename Container::value_type>());
        }

        template<typename Container2, typename Predicate>
        Iterator<Container> combine(Container2 other, Predicate predicate)
        {
          if (std::size(*this) != std::size(other)) {
              throw std::runtime_error("Both containers must have the same size");
          }

          Container result;

          auto it1 = std::begin(*this);
          auto it2 = std::begin(other);

          for (; it1!=std::end(*this); it1++, it2++) {
              result.insert(std::end(result), predicate(*it1, *it2));
          }

          return result;
        }

        template<typename Container2>
        Iterator<std::vector<std::pair<typename Container::value_type, typename Container2::value_type>>> pairs(Container2 other)
        {
          if (std::size(*this) != std::size(other)) {
              throw std::runtime_error("Both containers must have the same size");
          }

          std::vector<std::pair<typename Container::value_type, typename Container2::value_type>> result;

          auto it1 = std::begin(*this);
          auto it2 = std::begin(other);

          for (; it1!=std::end(*this); it1++, it2++) {
              result.insert(std::end(result), std::make_pair(*it1, *it2));
          }

          return result;
        }

        template<typename Container2>
          bool eq(Container2 other)
          {
            return std::equal(std::begin(*this), std::end(*this), std::begin(other));
          }

        template<typename Container2>
          bool ne(Container2 other)
          {
            return !eq(other);
          }

        template<typename Container2>
          bool lt(Container2 other)
          {
            return lexicographical_compare(std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }

        template<typename Container2>
          bool le(Container2 other)
          {
            if (eq(other) == true or lt(other) == true) {
              return true;
            }

            return false;
          }

        template<typename Container2>
          bool gt(Container2 other)
          {
            return !le(other);
          }

        template<typename Container2>
          bool ge(Container2 other)
          {
            return !lt(other);
          }

        template<typename Container2>
          bool cmp(Container2 other)
          {
            if (equal(other) == true) {
              return 0;
            }

            if (lt(other) == true) {
              return -1;
            }

            return +1;
          }

        template<typename Callback>
          Iterator<Container> & for_each(Callback callback, std::function<void()> begin = nullptr, std::function<void()> end = nullptr)
          {
            if (begin) {
              begin();
            }

            std::for_each(std::begin(*this), std::end(*this), callback);

            if (end) {
              end();
            }

            return *this;
          }

        template<typename Predicate, typename Initial>
          std::optional<Initial> zip(Predicate predicate, Initial value)
          {
            return std::accumulate(std::begin(*this), std::end(*this), value, predicate);
          }

        template<typename ResultContainer = Container, typename Predicate>
          Iterator<ResultContainer> map(Predicate predicate)
          {
            ResultContainer result;

            std::transform(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }

        Iterator<Container> & fill(typename Container::value_type value)
        {
            std::fill(std::begin(*this), std::end(*this), value);

            return *this;
        }

        Iterator<Container> skip(std::size_t n)
        {
          Container result;
          std::size_t i {0};

          std::copy_if(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), 
              [&i, n](auto const &item) {
              if (++i > n) {
                return true;
              }

                return false;
              });

          return Iterator<Container>(result);
        }

        Iterator<Container> take(std::size_t n)
        {
          Container result;
          std::size_t i {0};

          std::copy_if(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), 
              [&i, n](auto const &item) {
              if (i++ < n) {
                return true;
              }

                return false;
              });

          return Iterator<Container>(result);
        }

        Iterator<Container> chop(std::size_t n = 1)
        {
          Container result;

          std::copy(std::begin(*this), std::end(*this) - n, std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

        Iterator<Container> & reverse()
        {
          std::reverse(std::begin(*this), std::end(*this));

          return *this;
        }

        Iterator<Container> & shuffle()
        {
          std::random_device rd;
          std::mt19937 g(rd());

          std::shuffle(std::begin(*this), std::end(*this), g);

          return *this;
        }

        Iterator<Container> sample(std::size_t n)
        {
          Container result;

          if (n > std::size(*this)) {
            n = std::size(*this);
          }

          std::random_device rd;
          std::mt19937 g(rd());

          std::sample(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), n, g);

          return Iterator<Container>(result);
        }

        Iterator<Container> & lrotate(std::size_t n)
        {
          std::rotate(std::begin(*this), std::begin(*this) + n, std::end(*this));

          return *this;
        }

        Iterator<Container> & rrotate(std::size_t n)
        {
          std::rotate(std::rbegin(*this), std::rbegin(*this) + n, std::rend(*this));

          return *this;
        }

        std::vector<std::pair<std::size_t, typename Container::value_type>> enumerate()
        {
          std::vector<std::pair<std::size_t, typename Container::value_type>> result;
          std::size_t i {0};

          for (const auto &item : *this) {
            result.push_back(std::make_pair(i++, item));
          }

          return Iterator<std::vector<std::pair<std::size_t, typename Container::value_type>>>(result);
        }

        template<typename Container2>
          Iterator<Container> & swap(Container2 other)
          {
            std::swap(*this, other);

            return *this;
          }

        std::optional<typename Container::value_type> first()
        {
          if (std::size(*this) == 0) {
            return {};
          }

          return {*std::begin(*this)};
        }

        std::optional<typename Container::value_type> last()
        {
          if (std::size(*this) == 0) {
            return {};
          }

          return {*std::next(std::begin(*this), std::size(*this) - 1)};
        }

        std::optional<typename Container::value_type> nth(std::size_t n)
        {
          return {*std::next(std::begin(*this), n)};
        }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_first(Predicate predicate)
          {
            auto i = std::find_if(std::begin(*this), std::end(*this), predicate);

            if (i != std::end(*this)) {
              return {*i};
            }

            return {};
          }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_last(Predicate predicate)
          {
            auto last = std::end(*this);

            for (auto i=std::begin(*this); i!=std::end(*this); i++) {
              if (predicate(*i) == true) {
                last = i;
              }
            }

            if (last == std::end(*this)) {
              return {};
            }

            return {*last};
          }

        template<typename Compare = std::less<>>
          std::optional<typename Container::value_type> min(Compare compare = Compare())
          {
            auto i = std::min_element(std::begin(*this), std::end(*this), compare);

            if (i == std::end(*this)) {
              return {};
            }

            return {*i};
          }

        template<typename Compare = std::less<>>
          std::optional<typename Container::value_type> max(Compare compare = Compare())
          {
            auto i = std::max_element(std::begin(*this), std::end(*this), compare);

            if (i == std::end(*this)) {
              return {};
            }

            return {*i};
          }

        template<typename Predicate>
          std::optional<std::size_t> position(Predicate predicate)
          {
            auto i = std::find_if(std::begin(*this), std::end(*this), predicate);

            if (i != std::end(*this)) {
              return {std::distance(std::begin(*this), i)};
            }

            return {};
          }

        template<typename Predicate>
          bool all(Predicate predicate)
          {
            return std::all_of(std::begin(*this), std::end(*this), predicate);
          }

        template<typename Predicate>
          bool any(Predicate predicate)
          {
            return std::any_of(std::begin(*this), std::end(*this), predicate);
          }

        template<typename Predicate>
          bool none(Predicate predicate)
          {
            return std::none_of(std::begin(*this), std::end(*this), predicate);
          }

        template<typename Compare = std::less<>>
          Iterator<Container> & sort(Compare compare = Compare())
          {
            std::sort(std::begin(*this), std::end(*this), compare);

            return *this;
          }

        Iterator<Container> & unique()
        {
          if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
            throw std::runtime_error("Container must be sorted");
          }

          this->erase(std::unique(std::begin(*this), std::end(*this)), std::end(*this));

          return *this;
        }

        template<typename Container2>
          bool includes(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            return std::includes(std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }

        template<typename Container2>
          Iterator<Container> difference(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }
        
        template<typename Predicate>
        Iterator<Container> & generate(Predicate predicate, std::size_t n)
        {
          for (std::size_t i=0; i<n; i++) {
            this->insert(std::end(*this), predicate());
          }

          return *this;
        }
        
        template<typename Container2>
          Iterator<Container> complement(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(std::begin(other), std::end(other), std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> intersection(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_intersection(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> union_set(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_union(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> merge(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::merge(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> & append(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, std::end(*this)));

            return *this;
          }

        template<typename Container2>
          Iterator<Container> & prepend(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, std::begin(*this)));

            return *this;
          }

        Iterator<std::vector<Container>> breaks(std::size_t n)
        {
          std::vector<Container> result;

          for (const auto &item : *this) {
            if (result.size() == 0) {
              result.push_back(Container{});
            }

            Container *container = &result.back();

            if (container->size() >= n) {
              result.push_back(Container{});

              container = &result.back();
            }

            container->insert(std::begin(*container), item);
          }

          return Iterator<std::vector<Container>>{result};
        }

        Iterator<typename Container::value_type> flatten()
        {
          typename Container::value_type result;

          for (const auto &item : *this) {
            for (const auto &value : item) {
              result.push_back(value);
            }
          }

          return result;
        }

    };

}
