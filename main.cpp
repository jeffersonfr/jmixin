#include "jiterator.hpp"

#include <list>

int main()
{
    std::cout << jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
        .filter([](const auto &item) {
                if (item%2) {
                    return false;
                }

                return true;
                })
        .map<std::list<int>>([i=0](const auto &item) mutable {
                return ++i*item;
                })
        .reverse()
        .for_each([](const auto &item) {
                std::cout << "Item: " << item << std::endl;
                })
        .zip([](const auto &item1, const auto &item2) {
                return item1 + item2;
                }, 0).value_or(-1) << std::endl;

    return 0;
}
