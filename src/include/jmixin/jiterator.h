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

          std::copy(this->begin(), this->end(), std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

        template<typename Predicate>
          std::size_t count(Predicate predicate)
          {
            return std::count_if(this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          Iterator<Container> filter(Predicate predicate)
          {
            Container result;

            std::copy_if(this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

            return Iterator<Container>(result);
          }

        Iterator<Container> step(std::size_t n)
        {
          Container result;
          std::size_t i {0};

          std::copy_if(this->begin(), this->end(), std::inserter(result, std::end(result)), 
              [&i, n](auto const &) {
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

          return std::accumulate(this->begin(), this->end(), initial);
        }

        template<typename Initial>
        typename Container::value_type product(Initial initial = Initial())
        {
          Container result;

          return std::accumulate(this->begin(), this->end(), initial, std::multiplies<typename Container::value_type>());
        }

        template<typename Container2, typename Predicate>
        Iterator<Container> combine(Container2 other, Predicate predicate)
        {
          if (std::size(*this) != std::size(other)) {
            throw std::runtime_error("Both containers must have the same size");
          }

          Container result;

          auto it1 = this->begin();
          auto it2 = std::begin(other);

          for (; it1!=this->end(); it1++, it2++) {
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

          auto it1 = this->begin();
          auto it2 = std::begin(other);

          for (; it1!=this->end(); it1++, it2++) {
              result.insert(std::end(result), std::make_pair(*it1, *it2));
          }

          return result;
        }

        template<typename Container2>
          bool eq(Container2 other)
          {
            return std::equal(this->begin(), this->end(), std::begin(other));
          }

        template<typename Container2>
          bool ne(Container2 other)
          {
            return !eq(other);
          }

        template<typename Container2>
          bool lt(Container2 other)
          {
            return lexicographical_compare(this->begin(), this->end(), std::begin(other), std::end(other));
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

            std::for_each(this->begin(), this->end(), callback);

            if (end) {
              end();
            }

            return *this;
          }

        /*
        template<typename Callback, typename ExecutionPolicy>
          Iterator<Container> & for_each(ExecutionPolicy &&policy, Callback callback, std::function<void()> begin = nullptr, std::function<void()> end = nullptr)
          {
            if (begin) {
              begin();
            }

            std::for_each(policy, this->begin(), this->end(), callback);

            if (end) {
              end();
            }

            return *this;
          }
          */

        template<typename Predicate, typename Initial>
          std::optional<Initial> zip(Predicate predicate, Initial value)
          {
            return std::accumulate(this->begin(), this->end(), value, predicate);
          }

        template<typename ResultContainer = Container, typename Predicate>
          Iterator<ResultContainer> map(Predicate predicate)
          {
            ResultContainer result;

            std::transform(this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }

        template<typename Result = Container>
          Iterator<Result> build(std::function<Result(Container &)> f)
          {
            return f(*this);
          }

        Iterator<Container> & fill(typename Container::value_type value)
        {
            std::fill(this->begin(), this->end(), value);

            return *this;
        }

        Iterator<Container> skip(std::size_t n)
        {
          Container result;
          std::size_t i {0};

          std::copy_if(this->begin(), this->end(), std::inserter(result, std::end(result)), 
              [&i, n](auto const &) {
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

          std::copy_if(this->begin(), this->end(), std::inserter(result, std::end(result)), 
              [&i, n](auto const &) {
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

          std::copy(this->begin(), this->end() - n, std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

        Iterator<Container> & reverse()
        {
          std::reverse(this->begin(), this->end());

          return *this;
        }

        Iterator<Container> & shuffle()
        {
          std::random_device rd;
          std::mt19937 g(rd());

          std::shuffle(this->begin(), this->end(), g);

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

          std::sample(this->begin(), this->end(), std::inserter(result, std::end(result)), n, g);

          return Iterator<Container>(result);
        }

        Iterator<Container> & lrotate(std::size_t n)
        {
          std::rotate(this->begin(), this->begin() + n, this->end());

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

          return {*this->begin()};
        }

        std::optional<typename Container::value_type> last()
        {
          if (std::size(*this) == 0) {
            return {};
          }

          return {*std::next(this->begin(), std::size(*this) - 1)};
        }

        std::optional<typename Container::value_type> nth(std::size_t n)
        {
          return {*std::next(this->begin(), n)};
        }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_first(Predicate predicate)
          {
            auto i = std::find_if(this->begin(), this->end(), predicate);

            if (i != this->end()) {
              return {*i};
            }

            return {};
          }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_last(Predicate predicate)
          {
            auto last = this->end();

            for (auto i=this->begin(); i!=this->end(); i++) {
              if (predicate(*i) == true) {
                last = i;
              }
            }

            if (last == this->end()) {
              return {};
            }

            return {*last};
          }

        template<typename Compare = std::less<>>
          std::optional<typename Container::value_type> min(Compare compare = Compare())
          {
            auto i = std::min_element(this->begin(), this->end(), compare);

            if (i == this->end()) {
              return {};
            }

            return {*i};
          }

        template<typename Compare = std::less<>>
          std::optional<typename Container::value_type> max(Compare compare = Compare())
          {
            auto i = std::max_element(this->begin(), this->end(), compare);

            if (i == this->end()) {
              return {};
            }

            return {*i};
          }

        template<typename Predicate>
          std::optional<std::size_t> position(Predicate predicate)
          {
            auto i = std::find_if(this->begin(), this->end(), predicate);

            if (i != this->end()) {
              return {std::distance(this->begin(), i)};
            }

            return {};
          }

        template<typename Predicate>
          bool all(Predicate predicate)
          {
            return std::all_of(this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          bool any(Predicate predicate)
          {
            return std::any_of(this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          bool none(Predicate predicate)
          {
            return std::none_of(this->begin(), this->end(), predicate);
          }

        template<typename Compare = std::less<>>
          Iterator<Container> & sort(Compare compare = Compare())
          {
            std::sort(this->begin(), this->end(), compare);

            return *this;
          }

        Iterator<Container> & unique()
        {
          if (std::is_sorted(this->begin(), this->end()) == false) {
            throw std::runtime_error("Container must be sorted");
          }

          this->erase(std::unique(this->begin(), this->end()), this->end());

          return *this;
        }

        template<typename Container2>
          bool includes(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            return std::includes(this->begin(), this->end(), std::begin(other), std::end(other));
          }

        template<typename Container2>
          Iterator<Container> difference(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }
        
        template<typename Predicate>
        Iterator<Container> & generate(Predicate predicate, std::size_t n)
        {
          for (std::size_t i=0; i<n; i++) {
            this->insert(this->end(), predicate());
          }

          return *this;
        }
        
        template<typename Container2>
          Iterator<Container> complement(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(std::begin(other), std::end(other), this->begin(), this->end(), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> intersection(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_intersection(this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> union_set(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_union(this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> merge(Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::merge(this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> & append(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, this->end()));

            return *this;
          }

        template<typename Container2>
          Iterator<Container> & prepend(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, this->begin()));

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
        
        template<typename Predicate>
        Iterator<std::vector<Container>> partition(Predicate predicate)
        {
          std::vector<Container> result;

          result.push_back({});
          result.push_back({});

          auto it = std::partition(this->begin(), this->end(), predicate);

          std::copy(this->begin(), it, std::inserter(result[0], std::end(result[0])));
          std::copy(it, this->end(), std::inserter(result[1], std::end(result[1])));

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
