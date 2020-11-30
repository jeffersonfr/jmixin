#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>
#include <numeric>
#include <random>

#ifdef JMIXIN_JITERATOR_PARALLEL
#include <execution>
#endif

namespace jmixin {

  template<typename Container>
    class Iterator : public Container {

      public:
        Iterator() = default;

        Iterator(const Container &container):
          Container(container)
        {
        }

        Iterator<Container> copy() const
        {
          Container result;

          std::copy(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> copy(ExecutionPolicy &&policy) const
          {
            Container result;

            std::copy(policy, std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

            return Iterator<Container>(result);
          }
#endif

        Iterator<Container> copy_n(std::size_t n) const
        {
          Container result;

          std::copy_n(std::begin(*this), n, std::inserter(result, std::end(result)));

          return Iterator<Container>(result);
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> copy_n(ExecutionPolicy &&policy, std::size_t n) const
          {
            Container result;

            std::copy_n(policy, std::begin(*this), n, std::inserter(result, std::end(result)));

            return Iterator<Container>(result);
          }
#endif

        template<typename Predicate>
          std::size_t count(Predicate predicate) const
          {
            return std::count_if(std::begin(*this), std::end(*this), predicate);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          std::size_t count(ExecutionPolicy &&policy, Predicate predicate) const
          {
            return std::count_if(policy, std::begin(*this), std::end(*this), predicate);
          }
#endif

        template<typename Predicate>
          Iterator<Container> & filter(Predicate predicate)
          {
            this->erase(std::remove_if(std::begin(*this), std::end(*this), [&predicate](const auto &item) {return !predicate(item);}), std::end(*this));

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          Iterator<Container> & filter(ExecutionPolicy &&policy, Predicate predicate)
          {
            this->erase(std::remove_if(policy, std::begin(*this), std::end(*this), [&predicate](const auto &item) {return !predicate(item);}), std::end(*this));

            return *this;
          }
#endif

        Iterator<Container> & step(std::size_t n)
        {
          return this->filter([i=0, n](auto const &) mutable {
              if ((i++ % n) == 0) {
                return true;
              }

              return false;
          });
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
        Iterator<Container> & step(ExecutionPolicy policy, std::size_t n)
        {
          return this->filter(policy, [i=0, n](auto const &) mutable {
              if ((i++ % n) == 0) {
                return true;
              }

              return false;
          });
        }
#endif

        template<typename Initial>
          typename Container::value_type sum(Initial initial = Initial()) const
          {
            return std::reduce(std::begin(*this), std::end(*this), initial);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Initial, typename ExecutionPolicy>
          typename Container::value_type sum(ExecutionPolicy &&policy, Initial initial) const
          {
            return std::reduce(policy, std::begin(*this), std::end(*this), initial);
          }
#endif

        template<typename Initial>
          typename Container::value_type product(Initial initial = Initial()) const
          {
            return std::reduce(std::begin(*this), std::end(*this), initial, std::multiplies<typename Container::value_type>());
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Initial, typename ExecutionPolicy>
          typename Container::value_type product(ExecutionPolicy &&policy, Initial initial) const
          {
            return std::reduce(policy, std::begin(*this), std::end(*this), initial, std::multiplies<typename Container::value_type>());
          }
#endif

        template<typename Container2, typename Predicate>
          Iterator<Container> & combine(Container2 other, Predicate predicate)
          {
            if (std::size(*this) != std::size(other)) {
              throw std::runtime_error("Both containers must have the same size");
            }

            Iterator<Container> result;

            auto it1 = std::begin(*this);
            auto it2 = std::begin(other);

            for (; it1!=std::end(*this); it1++, it2++) {
              result.insert(std::end(result), predicate(*it1, *it2));
            }

            std::swap(*this, result);

            return *this;
          }

        template<typename Container2>
          Iterator<std::vector<std::pair<typename Container::value_type, typename Container2::value_type>>> pairs(Container2 other) const
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
          bool eq(Container2 other) const
          {
            return std::equal(std::begin(*this), std::end(*this), std::begin(other));
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool eq(ExecutionPolicy &&policy, Container2 other) const
          {
            return std::equal(policy, std::begin(*this), std::end(*this), std::begin(other));
          }
#endif

        template<typename Container2>
          bool ne(Container2 other) const
          {
            return !eq(other);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool ne(ExecutionPolicy &&policy, Container2 other) const
          {
            return !eq(policy, other);
          }
#endif

        template<typename Container2>
          bool lt(Container2 other) const
          {
            return lexicographical_compare(std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool lt(ExecutionPolicy &&policy, Container2 other) const
          {
            return lexicographical_compare(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }
#endif

        template<typename Container2>
          bool le(Container2 other) const
          {
            if (eq(other) == true or lt(other) == true) {
              return true;
            }

            return false;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool le(ExecutionPolicy &&policy, Container2 other) const
          {
            if (eq(policy, other) == true or lt(policy, other) == true) {
              return true;
            }

            return false;
          }
#endif

        template<typename Container2>
          bool gt(Container2 other) const
          {
            return !le(other);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool gt(ExecutionPolicy &&policy, Container2 other) const
          {
            return !le(policy, other);
          }
#endif

        template<typename Container2>
          bool ge(Container2 other) const
          {
            return !lt(other);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool ge(ExecutionPolicy &&policy, Container2 other) const
          {
            return !lt(policy, other);
          }
#endif

        template<typename Container2>
          bool cmp(Container2 other) const
          {
            if (equal(other) == true) {
              return 0;
            }

            if (lt(other) == true) {
              return -1;
            }

            return +1;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          bool cmp(ExecutionPolicy &&policy, Container2 other) const
          {
            if (equal(policy, other) == true) {
              return 0;
            }

            if (lt(policy, other) == true) {
              return -1;
            }

            return +1;
          }
#endif

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

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Callback, typename ExecutionPolicy>
          Iterator<Container> & for_each(ExecutionPolicy &&policy, Callback callback, std::function<void()> begin, std::function<void()> end)
          {
            if (begin) {
              begin();
            }

            std::for_each(policy, std::begin(*this), std::end(*this), callback);

            if (end) {
              end();
            }

            return *this;
          }
#endif

        template<typename Callback>
          Iterator<Container> & for_each_n(std::size_t n, Callback callback, std::function<void()> begin = nullptr, std::function<void()> end = nullptr)
          {
            if (begin) {
              begin();
            }

            std::for_each_n(std::begin(*this), n, callback);

            if (end) {
              end();
            }

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Callback, typename ExecutionPolicy>
          Iterator<Container> & for_each_n(ExecutionPolicy &&policy, std::size_t n, Callback callback, std::function<void()> begin, std::function<void()> end)
          {
            if (begin) {
              begin();
            }

            std::for_each_n(policy, std::begin(*this), n, callback);

            if (end) {
              end();
            }

            return *this;
          }
#endif

        template<typename Predicate, typename Initial>
          std::optional<Initial> zip(Predicate predicate, Initial value = Initial()) const
          {
            return std::reduce(std::begin(*this), std::end(*this), value, predicate);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename Initial, typename ExecutionPolicy>
          std::optional<Initial> zip(ExecutionPolicy &&policy, Predicate predicate, Initial value) const
          {
            return std::reduce(policy, std::begin(*this), std::end(*this), value, predicate);
          }
#endif

        template<typename ResultContainer = Container, typename Predicate>
          Iterator<ResultContainer> map(Predicate predicate) const
          {
            ResultContainer result;

            std::transform(std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ResultContainer = Container, typename Predicate, typename ExecutionPolicy>
          Iterator<ResultContainer> map(ExecutionPolicy &&policy, Predicate predicate) const
          {
            ResultContainer result;

            std::transform(policy, std::begin(*this), std::end(*this), std::inserter(result, std::end(result)), predicate);

            return Iterator<ResultContainer>(result);
          }
#endif

        template<typename Result = Container>
          Iterator<Result> build(std::function<Result(Container &)> f)
          {
            return f(*this);
          }

        Iterator<Container> & fill(typename Container::value_type value)
        {
          std::fill(std::begin(*this), std::end(*this), value);

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & fill(ExecutionPolicy &&policy, typename Container::value_type value)
          {
            std::fill(policy, std::begin(*this), std::end(*this), value);

            return *this;
          }
#endif

        Iterator<Container> & fill_n(std::size_t n, typename Container::value_type value)
        {
          std::fill_n(std::begin(*this), n, value);

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & fill_n(ExecutionPolicy &&policy, std::size_t n, typename Container::value_type value)
          {
            std::fill_n(policy, std::begin(*this), n, value);

            return *this;
          }
#endif

        Iterator<Container> & skip(std::size_t n = 1)
        {
          std::size_t i = 0;

          return this->filter([&i, n](auto const &) mutable {
              if (++i > n) {
                return true;
              }

              return false;
          });
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & skip(ExecutionPolicy &&policy, std::size_t n)
          {
            std::size_t i = 0;

            return this->filter(policy, [&i, n](auto const &) mutable {
                if (++i > n) {
                  return true;
                }

                return false;
            });
          }
#endif

        Iterator<Container> & take(std::size_t n)
        {
          std::size_t i = 0;

          return this->filter([&i, n](auto const &) mutable {
              if (i++ < n) {
                return true;
              }

              return false;
          });
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & take(ExecutionPolicy &&policy, std::size_t n)
          {
            std::size_t i = 0;

            return this->filter(policy, [&i, n](auto const &) mutable {
                if (i++ < n) {
                  return true;
                }

                return false;
            });
          }
#endif

        Iterator<Container> & chop(std::size_t n = 1)
        {
          if (n > std::size(*this)) {
            n = std::size(*this);
          }

          this->erase(std::next(std::end(*this), -n), std::end(*this));

          return *this;
        }

        Iterator<Container> & reverse()
        {
          std::reverse(std::begin(*this), std::end(*this));

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & reverse(ExecutionPolicy &&policy)
          {
            std::reverse(policy, std::begin(*this), std::end(*this));

            return *this;
          }
#endif

        Iterator<Container> & shuffle()
        {
          std::random_device rd;
          std::mt19937 g(rd());

          std::shuffle(std::begin(*this), std::end(*this), g);

          return *this;
        }

        Iterator<Container> sample(std::size_t n) const
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

        Iterator<Container> & left_rotate(std::size_t n)
        {
          std::rotate(std::begin(*this), std::next(std::begin(*this), n), std::end(*this));

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & left_rotate(ExecutionPolicy &&policy, std::size_t n)
          {
            std::rotate(policy, std::begin(*this), std::next(std::begin(*this), n), std::end(*this));

            return *this;
          }
#endif

        Iterator<Container> & right_rotate(std::size_t n)
        {
          std::rotate(this->rbegin(), std::next(this->rbegin(), n), this->rend());

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & right_rotate(ExecutionPolicy &&policy, std::size_t n)
          {
            std::rotate(policy, this->rbegin(), std::next(this->rbegin(), n), this->rend());

            return *this;
          }
#endif

        std::vector<std::pair<std::size_t, typename Container::value_type>> enumerate() const
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

        std::optional<typename Container::value_type> first() const
        {
          if (std::size(*this) == 0) {
            return {};
          }

          return {*std::begin(*this)};
        }

        std::optional<typename Container::value_type> last() const
        {
          if (std::size(*this) == 0) {
            return {};
          }

          return {*std::next(std::begin(*this), std::size(*this) - 1)};
        }

        std::optional<typename Container::value_type> nth(std::size_t n) const
        {
          return {*std::next(std::begin(*this), n)};
        }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_first(Predicate predicate) const
          {
            auto i = std::find_if(std::begin(*this), std::end(*this), predicate);

            if (i != std::end(*this)) {
              return {*i};
            }

            return {};
          }

        template<typename Predicate>
          std::optional<typename Container::value_type> find_last(Predicate predicate) const
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
          std::optional<typename Container::value_type> min(Compare compare = Compare()) const
          {
            auto i = std::min_element(std::begin(*this), std::end(*this), compare);

            if (i == std::end(*this)) {
              return {};
            }

            return {*i};
          }

        template<typename Compare = std::less<>>
          std::optional<typename Container::value_type> max(Compare compare = Compare()) const
          {
            auto i = std::max_element(std::begin(*this), std::end(*this), compare);

            if (i == std::end(*this)) {
              return {};
            }

            return {*i};
          }

        template<typename Predicate>
          std::optional<std::size_t> position(Predicate predicate) const
          {
            auto i = std::find_if(std::begin(*this), std::end(*this), predicate);

            if (i != std::end(*this)) {
              return {std::distance(std::begin(*this), i)};
            }

            return {};
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          std::optional<std::size_t> position(ExecutionPolicy &&policy, Predicate predicate) const
          {
            auto i = std::find_if(policy, std::begin(*this), std::end(*this), predicate);

            if (i != std::end(*this)) {
              return {std::distance(std::begin(*this), i)};
            }

            return {};
          }
#endif

        template<typename Predicate>
          bool all(Predicate predicate) const
          {
            return std::all_of(std::begin(*this), std::end(*this), predicate);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          bool all(ExecutionPolicy &&policy, Predicate predicate) const
          {
            return std::all_of(policy, std::begin(*this), std::end(*this), predicate);
          }
#endif

        template<typename Predicate>
          bool any(Predicate predicate) const
          {
            return std::any_of(std::begin(*this), std::end(*this), predicate);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          bool any(ExecutionPolicy &&policy, Predicate predicate) const
          {
            return std::any_of(policy, std::begin(*this), std::end(*this), predicate);
          }
#endif

        template<typename Predicate>
          bool none(Predicate predicate) const
          {
            return std::none_of(std::begin(*this), std::end(*this), predicate);
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          bool none(ExecutionPolicy &&policy, Predicate predicate) const
          {
            return std::none_of(policy, std::begin(*this), std::end(*this), predicate);
          }
#endif

        template<typename Compare = std::less<>>
          Iterator<Container> & sort(Compare compare = Compare())
          {
            std::sort(std::begin(*this), std::end(*this), compare);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Compare = std::less<>, typename ExecutionPolicy>
          Iterator<Container> & sort(ExecutionPolicy &&policy, Compare compare)
          {
            std::sort(policy, std::begin(*this), std::end(*this), compare);

            return *this;
          }
#endif

        Iterator<Container> & unique()
        {
          if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
            throw std::runtime_error("Container must be sorted");
          }

          this->erase(std::unique(std::begin(*this), std::end(*this)), std::end(*this));

          return *this;
        }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename ExecutionPolicy>
          Iterator<Container> & unique(ExecutionPolicy &&policy)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            this->erase(std::unique(policy, std::begin(*this), std::end(*this)), std::end(*this));

            return *this;
          }
#endif

        template<typename Container2>
          bool includes(Container2 other) const
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            return std::includes(std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2,typename ExecutionPolicy>
          bool includes(ExecutionPolicy &&policy, Container2 other) const
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            return std::includes(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other));
          }
#endif

        template<typename Container2>
          Iterator<Container> & difference(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_difference(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & difference(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_difference(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }
#endif

        template<typename Container2>
          Iterator<Container> & complement(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(std::begin(other), std::end(other), std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & complement(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Container result;

            std::set_difference(policy, std::begin(other), std::end(other), std::begin(*this), std::end(*this), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }
#endif

        template<typename Container2>
          Iterator<Container> & intersection(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_intersection(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & intersection(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_intersection(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }
#endif

        template<typename Container2>
          Iterator<Container> & union_set(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_union(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & union_set(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::set_union(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }
#endif

        template<typename Predicate>
          Iterator<Container> & generate(Predicate predicate, std::size_t n)
          {
            for (std::size_t i=0; i<n; i++) {
              this->insert(std::end(*this), predicate());
            }

            return *this;
          }

        template<typename Container2>
          Iterator<Container> & merge(Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::merge(std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & merge(ExecutionPolicy &&policy, Container2 other)
          {
            if (std::is_sorted(std::begin(*this), std::end(*this)) == false) {
              throw std::runtime_error("Container must be sorted");
            }

            Iterator<Container> result;

            std::merge(policy, std::begin(*this), std::end(*this), std::begin(other), std::end(other), std::inserter(result, std::end(result)));

            std::swap(*this, result);

            return *this;
          }
#endif

        template<typename Container2>
          Iterator<Container> & append(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, std::end(*this)));

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & append(ExecutionPolicy &&policy, Container2 other)
          {
            std::copy(policy, std::begin(other), std::end(other), std::inserter(*this, std::end(*this)));

            return *this;
          }
#endif

        template<typename Container2>
          Iterator<Container> & prepend(Container2 other)
          {
            std::copy(std::begin(other), std::end(other), std::inserter(*this, std::begin(*this)));

            return *this;
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Container2, typename ExecutionPolicy>
          Iterator<Container> & prepend(ExecutionPolicy &&policy, Container2 other)
          {
            std::copy(policy, std::begin(other), std::end(other), std::inserter(*this, std::begin(*this)));

            return *this;
          }
#endif

        Iterator<std::vector<Container>> breaks(std::size_t n) const
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

            auto it = std::partition(std::begin(*this), std::end(*this), predicate);

            std::copy(std::begin(*this), it, std::inserter(result[0], std::end(result[0])));
            std::copy(it, std::end(*this), std::inserter(result[1], std::end(result[1])));

            return Iterator<std::vector<Container>>{result};
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          Iterator<std::vector<Container>> partition(ExecutionPolicy &&policy, Predicate predicate)
          {
            std::vector<Container> result;

            result.push_back({});
            result.push_back({});

            auto it = std::partition(policy, std::begin(*this), std::end(*this), predicate);

            std::copy(policy, std::begin(*this), it, std::inserter(result[0], std::end(result[0])));
            std::copy(policy, it, std::end(*this), std::inserter(result[1], std::end(result[1])));

            return Iterator<std::vector<Container>>{result};
          }
#endif

        template<typename Predicate>
          Iterator<std::vector<Container>> stable_partition(Predicate predicate)
          {
            std::vector<Container> result;

            result.push_back({});
            result.push_back({});

            auto it = std::stable_partition(std::begin(*this), std::end(*this), predicate);

            std::copy(std::begin(*this), it, std::inserter(result[0], std::end(result[0])));
            std::copy(it, std::end(*this), std::inserter(result[1], std::end(result[1])));

            return Iterator<std::vector<Container>>{result};
          }

#ifdef JMIXIN_JITERATOR_PARALLEL
        template<typename Predicate, typename ExecutionPolicy>
          Iterator<std::vector<Container>> stable_partition(ExecutionPolicy &&policy, Predicate predicate)
          {
            std::vector<Container> result;

            result.push_back({});
            result.push_back({});

            auto it = std::stable_partition(policy, std::begin(*this), std::end(*this), predicate);

            std::copy(policy, std::begin(*this), it, std::inserter(result[0], std::end(result[0])));
            std::copy(policy, it, std::end(*this), std::inserter(result[1], std::end(result[1])));

            return Iterator<std::vector<Container>>{result};
          }
#endif

        Iterator<typename Container::value_type> flatten() const
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
