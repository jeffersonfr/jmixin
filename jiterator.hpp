#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <optional>
#include <numeric>

namespace jmixin {

template<typename Container>
class Iterator : public Container {

    public:
        Iterator(const Container &container):
            Container(container)
        {
        }

        template<typename Predicate>
        Iterator<Container> filter(Predicate predicate)
        {
            Container result;

            std::copy_if(std::begin(*this), std::end(*this), std::back_inserter(result), predicate);

            return Iterator<Container>(result);
        }

        Iterator<Container> step(std::size_t n)
        {
            Container result;
            std::size_t i {0};

            std::copy_if(std::begin(*this), std::end(*this), std::back_inserter(result), 
                    [&i, n](auto const &item) {
                        if ((i++ % n) == 0) {
                            return true;
                        }

                        return false;
                    });

            return Iterator<Container>(result);
        }

        typename Container::value_type sum()
        {
            Container result;

            return std::accumulate(std::begin(*this), std::end(*this));
        }

        typename Container::value_type product()
        {
            Container result;

            return std::accumulate(std::begin(*this), std::end(*this), 1, std::multiplies<typename Container::value_type>());
        }

        template<typename Container2>
        bool eq(Container2 other)
        {
            return std::equal(std::begin(*this), std::end(*this), std::begin(other));
        }

        template<typename Container2>
        bool neq(Container2 other)
        {
            return !eq(other);
        }

        template<typename Container2>
        bool lt(Container2 other)
        {
            if (lexicographical_compare(std::begin(*this), std::end(*this), std::begin(other), std::end(other)) == true) {
                return true;
            }

            return false;
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

            if (lexicographical_compare(std::begin(*this), std::end(*this), std::begin(other), std::end(other)) == true) {
                return -1;
            }

            return +1;
        }

        template<typename Predicate, typename Initial>
        std::optional<typename Container::value_type> zip(Predicate predicate, Initial value)
        {
            return std::accumulate(std::begin(*this), std::end(*this), value, predicate);
        }

        template<typename Predicate>
        Iterator<Container> map(Predicate predicate)
        {
            Container result;

            std::transform(std::begin(*this), std::end(*this), std::back_inserter(result), predicate);

            return Iterator<Container>(result);
        }

        template<typename Callback>
        Iterator<Container> for_each(Callback callback)
        {
            std::for_each(std::begin(*this), std::end(*this), callback);

            return *this;
        }

        Iterator<Container> skip(std::size_t n)
        {
            Container result;
            std::size_t i {0};

            std::copy_if(std::begin(*this), std::end(*this), std::back_inserter(result), 
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

            std::copy_if(std::begin(*this), std::end(*this), std::back_inserter(result), 
                    [&i, n](auto const &item) {
                        if (i++ < n) {
                            return true;
                        }

                        return false;
                    });

            return Iterator<Container>(result);
        }

        Iterator<Container> reverse()
        {
            Container result;

            std::for_each(std::begin(*this), std::end(*this),
                    [&result](const auto &item) {
                        result.insert(std::begin(result), item);
                    });

            return Iterator<Container>(result);
        }

        Iterator<Container> copy()
        {
            Container result;

            std::copy(std::begin(*this), std::end(*this), std::back_inserter(result));

            return Iterator<Container>(result);
        }

        std::optional<typename Container::value_type> first()
        {
            if (this->size() == 0) {
                return {};
            }

            return {std::begin(*this)};
        }

        std::optional<typename Container::value_type> last()
        {
            if (this->size() == 0) {
                return {};
            }

            auto i=std::begin(*this);

            for (; i!=std::end(*this); i++) {
                auto next = ++i;
    
                if (next == std::end(*this)) {
                    break;
                }
            }

            return {*i};
        }

        std::optional<typename Container::value_type> nth(std::size_t n)
        {
            return {std::next(std::begin(*this), n)};
        }

        template<typename Predicate>
        std::optional<typename Container::value_type> find(Predicate predicate)
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

        template<typename Predicate>
        std::optional<typename Container::value_type> max()
        {
            auto i = std::max_element(std::begin(*this), std::end(*this));

            if (i == std::end(*this)) {
                return {};
            }

            return {*i};
        }

        template<typename Predicate>
        std::optional<typename Container::value_type> min()
        {
            auto i = std::min_element(std::begin(*this), std::end(*this));

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
                return {i - std::begin(*this)};
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
            return std::all_of(std::begin(*this), std::end(*this), predicate);
        }

        Iterator<Container> sort()
        {
            std::sort(std::begin(*this), std::end(*this));

            return *this;
        }

};

}
