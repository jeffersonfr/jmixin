#include "jmixin/jiterator.h"

#include <list>
#include <map>

void test1()
{
    std::cout << "--> test1" << std::endl;

    jmixin::Iterator(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
        .filter([](const auto &item) {
                if (item%2) {
                    return false;
                }

                return true;
                })
        .map<std::map<int, int>>([i=0](const auto &item) mutable {
                return std::make_pair(i++, item);
                })
        .for_each([](const auto &item) {
                std::cout << "Item: " << item.first << ", " << item.second << std::endl;
                })
        .map<std::list<std::string>>([](const auto &item) {
                return std::to_string(item.first*item.second) + "us";
                })
        .for_each([](const auto &item) {
                std::cout << "Item2: " << item << std::endl;
                });
}

void test2()
{
    std::cout << "--> test2" << std::endl;

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
}

void test3()
{
    std::cout << "--> test3" << std::endl;

    jmixin::Iterator(std::vector<int>{5, 3, 1, 6, 7, 8, 3, 1, 4})
        .sort()
        .for_each([](const auto &item) {
                std::cout << "Item: " << item << std::endl;
                });
}

int main()
{
    test1();
    test2();
    test3();

    return 0;
}
