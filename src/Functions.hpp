#ifndef H_FUNCTIONS
#define H_FUNCTIONS

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>


#include "Video.hpp"

namespace fs = std::filesystem;

void sort_folders();
std::vector<Video> get_files(const std::string& dir, const std::string& ext, const std::vector<std::string> exclude);
void load_state(std::vector<Video>& videos, const std::string& folders, const std::string& exclude, const std::string& old_list = "");
void save_state(std::vector<Video>& videos, const std::string list_file);

#endif
