#include "jmixin/jiterator.h"
#include "jmixin/jstring.h"

#include <sstream>
#include <string>
#include <fstream>

static const std::string gFile {"../src/tests/megasena.txt"};

std::vector<std::vector<std::size_t>> load_results(const std::string &path)
{
  std::ifstream in;

  in.open(path);

  if (!in) {
    std::clog << "unable to open file '" << gFile << "'" << std::endl;

    return {};
  }

  std::vector<std::vector<std::size_t>> results;
  std::string line;

  while (std::getline(in, line)) {
    std::istringstream is(line);

    std::vector<std::size_t> values;

    for (std::size_t i = 0; i < 6; i++) {
      std::size_t value;

      is >> value;

      values.push_back(value);
    }
    
    results.push_back(values);
  }

  return results;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <range of average results>" << std::endl;

    return 0;
  }

  auto numberOfAverageResults = std::atoi(argv[1]);

  auto results = load_results(gFile);

  std::map<std::size_t, std::size_t> freqMaximun;

  jmixin::Iterator(results)
    .map<std::vector<std::size_t>>([](const std::vector<std::size_t> &item) {
        return jmixin::Iterator(item).sum<std::size_t>();
    }).for_each([&freqMaximun](const auto &item) {
      freqMaximun[item]++;
    });

  auto average = jmixin::Iterator(freqMaximun)
    .map<std::vector<std::pair<std::size_t, std::size_t>>>([](const auto &item) {
        return item;})
    .sort([](const auto &item1, const auto &item2) {
        return item1.second > item2.second;})
    .take(numberOfAverageResults).for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.first)).right(3) << " ";
    }, []{std::cout << "\n:maximun sums: " << std::endl;}, []{std::cout << std::endl;})
    .for_each([](const auto &) {
        std::cout << "--- ";
    }, []{}, []{std::cout << std::endl;})
    .for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.second)).right(3) << " ";
    }, []{}, []{std::cout << std::endl;})
    .map<std::vector<std::size_t>>([](const auto &item) {
        return item.first;
    });

  std::map<std::size_t, std::size_t> freq;
  std::size_t filteredGames;

  filteredGames = jmixin::Iterator(results)
    .filter([&average](const std::vector<std::size_t> &item) {
      auto sum = jmixin::Iterator(item).sum<std::size_t>();

      return average.any([sum](const auto &item) {
        return item == sum;
      });
    }).flatten().for_each([&freq](const auto &item) {
      freq[item]++;
    }).size()/6;

  std::cout << "\n" << filteredGames << " games of " << results.size() << " resulting in " << 100*filteredGames/results.size() << "%" << std::endl;

  jmixin::Iterator(freq)
    .map<std::vector<std::pair<std::size_t, std::size_t>>>([](const auto &item) {
        return item;})
    .sort([](const auto &item1, const auto &item2) {
        return item1.first < item2.first;})
    .for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.first)).right(2) << " ";
    }, []{std::cout << "\n:histogram: " << std::endl;}, []{std::cout << std::endl;})
    .for_each([](const auto &) {
        std::cout << "-- ";
    }, []{}, []{std::cout << std::endl;})
    .for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.second)).right(2) << " ";
    }, []{}, []{std::cout << std::endl;});

  auto gen = jmixin::Iterator(std::vector<std::size_t>{}).generate([i=1]() mutable {return i++;}, 60);

  jmixin::Iterator(freq)
    .map<std::vector<std::size_t>>([](const auto &item) {
        return item.first;})
    .complement(gen)
    .sort().for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item)).right(2) << " ";
    }, []{std::cout << "\n:excluded numbers: ";}, []{std::cout << std::endl;});

  auto sumValues = jmixin::Iterator(freq)
    .map<std::vector<std::pair<std::size_t, std::size_t>>>([](const auto &item) {
        return item;})
    .sort([](const auto &item1, const auto &item2) {
        return item1.second < item2.second;})
    .for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.first)).right(2) << " ";
    }, []{std::cout << "\n:histogram [sorted by value]: " << std::endl;}, []{std::cout << std::endl;})
    .for_each([](const auto &) {
        std::cout << "-- ";
    }, []{}, []{std::cout << std::endl;})
    .for_each([](const auto &item) {
        std::cout << jmixin::String(std::to_string(item.second)).right(2) << " ";
    }, []{}, []{std::cout << std::endl;})
    .map<std::vector<std::size_t>>([](const auto &item) {
        return item.second;
    }).sum<std::size_t>();

  auto valuesRange = jmixin::Iterator(freq)
    .for_each([sumValues](const auto &item) {
        std::cout << item.first << " = " << 100.0*item.second/sumValues << std::endl;
    }, []{std::cout << "\n:probabilities: " << std::endl;}, []{})
    .map<std::vector<std::size_t>>([](const auto &item) {
        return item.second;
    }).sum<std::size_t>();

  std::cout << "\n:sample games:" << std::endl;

  for (int i=0; i<10; i++)
    jmixin::Iterator(std::vector<std::size_t>{})
      .generate([valuesRange]() {
          std::random_device rd;
          std::mt19937 g(rd());

          return g()%valuesRange;
      }, 6).sort().for_each([freq](auto &item) {
        auto total = 0;

        for (auto value : freq) {
          if (value.second < item) {
            total = total + value.second;
            item = item - value.second;
          } else {
            std::cout << value.first << " ";

            return;
          }
        }
      }, []{}, []{std::cout << std::endl;});

  return 0;
}

