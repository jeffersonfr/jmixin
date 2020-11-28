#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>
#include <numeric>
#include <random>
#include <execution>

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
          return this->copy(std::execution::seq);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> copy(ExecutionPolicy &&policy)
          {
            Container result;

            std::copy(policy, this->begin(), this->end(), std::inserter(result, std::end(result)));

            return Iterator<Container>(result);
          }

        template<typename Predicate>
          std::size_t count(Predicate predicate)
          {
            return this->count(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          std::size_t count(ExecutionPolicy &&policy, Predicate predicate)
          {
            return std::count_if(policy, this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          Iterator<Container> filter(Predicate predicate)
          {
            /*
               Container result;

               std::copy_if(this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

               return Iterator<Container>(result);
               */

            this->erase(std::remove_if(this->begin(), this->end(), [&predicate](const auto &item) {return !predicate(item);}), this->end());

            return *this;
          }

        template<typename Predicate, typename ExecutionPolicy>
          Iterator<Container> filter(ExecutionPolicy &&policy, Predicate predicate)
          {
            /*
               Container result;

               std::copy_if(policy, this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

               return Iterator<Container>(result);
               */

            this->erase(std::remove_if(policy, this->begin(), this->end(), [&predicate](const auto &item) {return !predicate(item);}), this->end());

            return *this;
          }

        Iterator<Container> step(std::size_t n)
        {
          return this->filter([i=0, n](auto const &) mutable {
              if ((i++ % n) == 0) {
                return true;
              }

              return false;
              });
        }

        template<typename Initial>
          typename Container::value_type sum(Initial initial = Initial())
          {
            return this->sum(std::execution::seq, initial);
          }

        template<typename Initial, typename ExecutionPolicy>
          typename Container::value_type sum(ExecutionPolicy &&policy, Initial initial)
          {
            Container result;

            return std::reduce(policy, this->begin(), this->end(), initial);
          }

        template<typename Initial>
          typename Container::value_type product(Initial initial = Initial())
          {
            return this->product(std::execution::seq, initial);
          }

        template<typename Initial, typename ExecutionPolicy>
          typename Container::value_type product(ExecutionPolicy &&policy, Initial initial)
          {
            Container result;

            return std::reduce(policy, this->begin(), this->end(), initial, std::multiplies<typename Container::value_type>());
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
            return this->eq(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool eq(ExecutionPolicy &&policy, Container2 other)
          {
            return std::equal(policy, this->begin(), this->end(), std::begin(other));
          }

        template<typename Container2>
          bool ne(Container2 other)
          {
            return this->ne(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool ne(ExecutionPolicy &&policy, Container2 other)
          {
            return !eq(policy, other);
          }

        template<typename Container2>
          bool lt(Container2 other)
          {
            return this->lt(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool lt(ExecutionPolicy &&policy, Container2 other)
          {
            return lexicographical_compare(policy, this->begin(), this->end(), std::begin(other), std::end(other));
          }

        template<typename Container2>
          bool le(Container2 other)
          {
            return this->le(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool le(ExecutionPolicy &&policy, Container2 other)
          {
            if (eq(policy, other) == true or lt(policy, other) == true) {
              return true;
            }

            return false;
          }

        template<typename Container2>
          bool gt(Container2 other)
          {
            return this->gt(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool gt(ExecutionPolicy &&policy, Container2 other)
          {
            return !le(policy, other);
          }

        template<typename Container2>
          bool ge(Container2 other)
          {
            return this->ge(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool ge(ExecutionPolicy &&policy, Container2 other)
          {
            return !lt(policy, other);
          }

        template<typename Container2>
          bool cmp(Container2 other)
          {
            return this->cmp(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          bool cmp(ExecutionPolicy &&policy, Container2 other)
          {
            if (equal(policy, other) == true) {
              return 0;
            }

            if (lt(policy, other) == true) {
              return -1;
            }

            return +1;
          }

        template<typename Callback>
          Iterator<Container> & for_each(Callback callback, std::function<void()> begin = nullptr, std::function<void()> end = nullptr)
          {
            return this->for_each(std::execution::seq, callback, begin, end);
          }

        template<typename Callback, typename ExecutionPolicy>
          Iterator<Container> & for_each(ExecutionPolicy &&policy, Callback callback, std::function<void()> begin, std::function<void()> end)
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

        template<typename Predicate, typename Initial>
          std::optional<Initial> zip(Predicate predicate, Initial value = Initial())
          {
            return this->zip(std::execution::seq, predicate, value);
          }

        template<typename Predicate, typename Initial, typename ExecutionPolicy>
          std::optional<Initial> zip(ExecutionPolicy &&policy, Predicate predicate, Initial value)
          {
            return std::reduce(policy, this->begin(), this->end(), value, predicate);
          }

        template<typename ResultContainer = Container, typename Predicate>
          Iterator<ResultContainer> map(Predicate predicate)
          {
            ResultContainer result;

            std::transform(this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }

        template<typename ResultContainer = Container, typename Predicate, typename ExecutionPolicy>
          Iterator<ResultContainer> map(ExecutionPolicy &&policy, Predicate predicate)
          {
            ResultContainer result;

            std::transform(policy, this->begin(), this->end(), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }

        template<typename Result = Container>
          Iterator<Result> build(std::function<Result(Container &)> f)
          {
            return f(*this);
          }

        Iterator<Container> & fill(typename Container::value_type value)
        {
          return this->fill(std::execution::seq, value);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> & fill(ExecutionPolicy &&policy, typename Container::value_type value)
          {
            std::fill(policy, this->begin(), this->end(), value);

            return *this;
          }

        Iterator<Container> skip(std::size_t n = 1)
        {
          return this->skip(std::execution::seq, n);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> skip(ExecutionPolicy &&policy, std::size_t n)
          {
            std::size_t i = 0;

            return this->filter([&i, n](auto const &) mutable {
                if (++i > n) {
                  return true;
                }

                return false;
                });
          }

        Iterator<Container> take(std::size_t n)
        {
          return this->take(std::execution::seq, n);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> take(ExecutionPolicy &&policy, std::size_t n)
          {
            std::size_t i = 0;

            return this->filter(policy, [&i, n](auto const &) mutable {
                if (i++ < n) {
                  return true;
                }

                return false;
                });
          }

        Iterator<Container> chop(std::size_t n = 1)
        {
          return this->chop(std::execution::seq, n);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> chop(ExecutionPolicy &&policy, std::size_t n)
          {
            Container result;

            std::copy(policy, this->begin(), this->end() - n, std::inserter(result, std::end(result)));

            return Iterator<Container>(result);
          }

        Iterator<Container> & reverse()
        {
          return this->reverse(std::execution::seq);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> & reverse(ExecutionPolicy &&policy)
          {
            std::reverse(policy, this->begin(), this->end());

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
          return this->lrotate(std::execution::seq, n);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> & lrotate(ExecutionPolicy &&policy, std::size_t n)
          {
            std::rotate(policy, this->begin(), this->begin() + n, this->end());

            return *this;
          }

        Iterator<Container> & rrotate(std::size_t n)
        {
          return this->rrotate(std::execution::seq, n);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> & rrotate(ExecutionPolicy &&policy, std::size_t n)
          {
            std::rotate(policy, this->rbegin(), this->rbegin() + n, this->rend());

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
            return this->position(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          std::optional<std::size_t> position(ExecutionPolicy &&policy, Predicate predicate)
          {
            auto i = std::find_if(policy, this->begin(), this->end(), predicate);

            if (i != this->end()) {
              return {std::distance(this->begin(), i)};
            }

            return {};
          }

        template<typename Predicate>
          bool all(Predicate predicate)
          {
            return this->all(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          bool all(ExecutionPolicy &&policy, Predicate predicate)
          {
            return std::all_of(policy, this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          bool any(Predicate predicate)
          {
            return this->any(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          bool any(ExecutionPolicy &&policy, Predicate predicate)
          {
            return std::any_of(policy, this->begin(), this->end(), predicate);
          }

        template<typename Predicate>
          bool none(Predicate predicate)
          {
            return this->none(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          bool none(ExecutionPolicy &&policy, Predicate predicate)
          {
            return std::none_of(policy, this->begin(), this->end(), predicate);
          }

        template<typename Compare = std::less<>>
          Iterator<Container> & sort(Compare compare = Compare())
          {
            return this->sort(std::execution::seq, compare);
          }

        template<typename Compare = std::less<>, typename ExecutionPolicy>
          Iterator<Container> & sort(ExecutionPolicy &&policy, Compare compare)
          {
            std::sort(policy, this->begin(), this->end(), compare);

            return *this;
          }

        Iterator<Container> & unique()
        {
          return this->unique(std::execution::seq);
        }

        template<typename ExecutionPolicy>
          Iterator<Container> & unique(ExecutionPolicy &&policy)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            this->erase(std::unique(policy, this->begin(), this->end()), this->end());

            return *this;
          }

        template<typename Container2>
          bool includes(Container2 other)
          {
            return this->includes(std::execution::seq, other);
          }

        template<typename Container2,typename ExecutionPolicy>
          bool includes(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            return std::includes(policy, this->begin(), this->end(), std::begin(other), std::end(other));
          }

        template<typename Container2>
          Iterator<Container> difference(Container2 other)
          {
            return this->difference(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> difference(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(policy, this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> complement(Container2 other)
          {
            return this->complement(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> complement(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(policy, std::begin(other), std::end(other), this->begin(), this->end(), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> intersection(Container2 other)
          {
            return this->intersection(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> intersection(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_intersection(policy, this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> union_set(Container2 other)
          {
            return this->union_set(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> union_set(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_union(policy, this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

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
          Iterator<Container> merge(Container2 other)
          {
            return this->merge(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> merge(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(this->begin(), this->end()) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::merge(policy, this->begin(), this->end(), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            return result;
          }

        template<typename Container2>
          Iterator<Container> & append(Container2 other)
          {
            return this->append(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & append(ExecutionPolicy &&policy, Container2 other)
          {
            std::copy(policy, std::begin(other), std::end(other), std::inserter(*this, this->end()));

            return *this;
          }

        template<typename Container2>
          Iterator<Container> & prepend(Container2 other)
          {
            return this->prepend(std::execution::seq, other);
          }

        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & prepend(ExecutionPolicy &&policy, Container2 other)
          {
            std::copy(policy, std::begin(other), std::end(other), std::inserter(*this, this->begin()));

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
            return this->partition(std::execution::seq, predicate);
          }

        template<typename Predicate, typename ExecutionPolicy>
          Iterator<std::vector<Container>> partition(ExecutionPolicy &&policy, Predicate predicate)
          {
            std::vector<Container> result;

            result.push_back({});
            result.push_back({});

            auto it = std::partition(policy, this->begin(), this->end(), predicate);

            std::copy(policy, this->begin(), it, std::inserter(result[0], std::end(result[0])));
            std::copy(policy, it, this->end(), std::inserter(result[1], std::end(result[1])));

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
