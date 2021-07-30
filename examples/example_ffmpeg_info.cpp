#include "jmixin/jiterator.h"
#include "jmixin/jstring.h"

#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

const static std::size_t gcsLineSize = 4096;

struct jelementary_stream_t {
  std::size_t index;
  std::size_t pid;
  std::string type;
  std::string description;
};

struct jprogram_t {
  std::size_t number;
  std::string name;
  jmixin::Iterator<std::vector<jelementary_stream_t>> streams;
};

struct jmedia_t {
  std::string filename;
  std::chrono::milliseconds duration;
  jmixin::Iterator<std::vector<jprogram_t>> programs;
};

std::vector<std::string> mediainfo(const std::string &path)
{
  std::ostringstream o;

  o << "ffmpeg -i \"" << path << "\" 2>&1 | sed \"0,/Input #0, mpegts/d\"";

  FILE *info = popen(o.str().c_str(), "r");

  if (info == nullptr) {
    return {};
  }

  std::vector<std::string> result;
  char *line = new char[gcsLineSize];
  std::size_t lineSize = gcsLineSize;

  while (getline(&line, &lineSize, info) != -1) {
    result.emplace_back(line);
  }

  free(line);
  fclose(info);

  return result;
}

jmixin::Iterator<std::vector<jmedia_t>> parseMediaFiles(const std::vector<std::string> &args)
{
  jmixin::Iterator<std::vector<jmedia_t>> medias;

  for (auto &arg : args) {
    auto info = mediainfo(arg);

    if (info.size() == 0) {
      std::clog << "Unable to parse media file, probably isn't a MPEG-TS" << std::endl;

      continue;
    }

    jmedia_t media;

    media.filename = arg;

    auto duration = jmixin::String(*info.begin()).replace_groups("Duration: (..:..:..\\...).*", "$1").trim();

    struct tm tm;

    if (strptime(duration.c_str(), "%T", &tm)) {
      media.duration = std::chrono::milliseconds{(tm.tm_hour*3600 + tm.tm_min*60 + tm.tm_sec)*1000};
    }

    info.erase(info.begin());

    std::clog << "duration: " << media.duration.count() << "\n" << std::endl;

    while (jmixin::String(*info.begin()).starts_with("  Program") == true) {
      jprogram_t program;

      program.number = std::atoi(jmixin::String(*info.begin()).replace_groups("Program (.*)", "$1").trim().c_str());

      info.erase(info.begin()); // program

      info.erase(info.begin()); // metadata

      program.name = jmixin::String(*info.begin()).replace_groups("service_name.*: (.*)", "$1").trim();
    
      info.erase(info.begin()); // serice_name

      info.erase(info.begin()); // serice_provider

      std::clog << "program: " << program.number << ": " << program.name << std::endl;

      while (jmixin::String(*info.begin()).starts_with("    Stream") == true) {
        auto streamInfo = jmixin::String(*info.begin()).groups("Stream \\#0:([[:d:]]*)\\[([[:alnum:]]*)\\].*?: ([[:alnum:]]*): (.*)");

        info.erase(info.begin()); // stream

        if (streamInfo[0].size() == 5) { // 1<match> + 4<sub-groups>
          jelementary_stream_t stream;

          stream.index = std::atoi(streamInfo[0][1].trim().c_str());
          stream.pid = std::atoi(streamInfo[0][2].trim().c_str());
          stream.type = streamInfo[0][3].trim();
          stream.description = streamInfo[0][4].trim();
       
          program.streams.emplace_back(stream);

          std::clog << stream.index << ":" << stream.pid << ":" << stream.type << ": " << stream.description << std::endl;
        }
      }

      media.programs.emplace_back(program);

      std::cout << std::endl;
    }

    medias.emplace_back(media);
  }

  return medias;
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::clog << "usage: " << argv[0] << " <media>" << std::endl;

    return 0;
  }

  std::vector<std::string> args;

  for (int i=1; i<=argc-1; i++) {
    args.emplace_back(argv[i]);
  }
 
  auto result = parseMediaFiles(args);
  
  return 0;
}

