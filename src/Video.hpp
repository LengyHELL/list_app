#ifndef H_VIDEO
#define H_VIDEO

#include <filesystem>

struct Video {
  Video(const std::string& name, const std::string& path) : name(name), path(path) {}
  Video(const std::string& name, const std::filesystem::path& path) : name(name), path(path) {}

  std::string name;
  std::filesystem::path path;
};

#endif
