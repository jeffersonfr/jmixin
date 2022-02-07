#include "jmixin/jiterator.h"
#include "jmixin/jstring.h"

#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

struct jprocess_t {
  std::string path;
  std::string name;
  std::string args;
  std::size_t pid;
  std::size_t uid;
  std::size_t gid;
};

uid_t userid_from_name(const std::string &name)
{
  if (jmixin::String(name).is_number() == true) {
    return std::atoi(name.c_str());
  }

  struct passwd *pwd = getpwnam(name.c_str());
  
  if (pwd == nullptr) {
    return -1;
  }
  
  return pwd->pw_uid;
}

std::vector<std::string> read_lines_from_file(const std::string &path)
{
  std::ifstream in;

  in.open(path);

  if (!in) {
    std::clog << "unable to open file '" << path << "'" << std::endl;

    return {};
  }

  std::vector<std::string> results;
  std::string line;

  while (std::getline(in, line)) {
    results.push_back(line);
  }

  return results;
}

std::vector<std::string> load_pids_from_proc()
{
  std::vector<std::string> pids;

  for (const auto &entry : std::filesystem::directory_iterator("/proc")) {
    if (jmixin::String(entry.path().string()).match("/proc/[[:d:]]*$") == true) {
      pids.push_back(entry.path().string());
    }
  }

  return pids;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::clog << "usage: " << argv[0] << " <username>" << std::endl;

    return 0;
  }

  std::size_t userid = userid_from_name(argv[1]);

  jmixin::Iterator(load_pids_from_proc())
    .template map<std::vector<jprocess_t>>([userid](const auto &pid_path) {
      jprocess_t process {};

      process.path = pid_path;

      jmixin::Iterator(read_lines_from_file(pid_path + "/status"))
        .for_each([&process](const auto &item) {
          std::istringstream is(item);
          std::string str;

          if (jmixin::String(item).lower_case().starts_with("name:") == true) {
            is >> str >> str;

            process.name = str;
          }

          if (jmixin::String(item).lower_case().starts_with("uid:") == true) {
            is >> str >> str;

            process.uid = std::atoi(str.c_str());
          }
          
          if (jmixin::String(item).lower_case().starts_with("gid:") == true) {
            is >> str >> str;

            process.gid = std::atoi(str.c_str());
          }
          
          if (jmixin::String(item).lower_case().starts_with("pid:") == true) {
            is >> str >> str;

            process.pid = std::atoi(str.c_str());
          }
        });

      jmixin::Iterator(read_lines_from_file(pid_path + "/cmdline"))
        .for_each([&process](const auto &item) {
          process.args = item;
        });

      return process;
    }).filter([userid](const auto &process) {
      return userid == process.uid;
    }).for_each([](const auto &process) {
      std::cout << jmixin::String(std::to_string(process.pid)).right(8) << ": " << process.name << " " << process.args << std::endl;
    });

  return 0;
}

