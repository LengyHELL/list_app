#include "Functions.hpp"

void sort_folders() {
  for (const auto& entry : fs::directory_iterator(".")) {
    fs::path current = entry.path();
    fs::path best;

    if (!fs::is_directory(current) && (current.extension() == ".mkv")) {
      std::string filename = current.stem().string();

      for (const auto& entry2 : fs::directory_iterator(".")) {
        fs::path current2 = entry2.path();

        if (fs::is_directory(current2)) {
          std::string folder = current2.stem().string();

          if (filename.find(folder) != std::string::npos) {
            if (current2.filename().string().size() > best.filename().string().size()) {
              best = current2;
            }
          }
        }
      }
      if (!best.empty()) {
        fs::path new_path = best.string() + "/" + current.filename().string();
        fs::rename(entry.path(), new_path);
      }
    }
  }
}

std::vector<Video> get_files(const std::string& dir, const std::string& ext, const std::vector<std::string> exclude) {
  std::vector<Video> vec;

  if (fs::is_directory(fs::path(dir))) {
    for (const auto& entry : fs::directory_iterator(dir)) {
      fs::path current = entry.path();
      if (!fs::is_directory(current) && (current.extension() == ".mkv")) {
        if (std::find(exclude.begin(), exclude.end(), current.string()) == exclude.end()) {
          vec.push_back(Video(current.stem().string(), current));
        }
      }
    }
  }
  return vec;
}

void load_state(std::vector<Video>& videos, const std::string& folders, const std::string& exclude, const std::string& old_list) {
  std::vector<std::string> exclude_list;

  if ((old_list != "") && (exclude != "")) {
    std::ifstream ml(old_list);
    if (ml.is_open()) {
      std::fstream ex(exclude);
      if (ex.is_open()) {
        ex.seekg(0, std::ios::end);
        std::string line;

        while(getline(ml, line)) {
          ex << line << '\n';
        }
        ex.close();
      }
      ml.close();
    }
  }

  if (exclude != "") {
    std::ifstream ex(exclude);
    if (ex.is_open()) {
      std::string line;
      while(getline(ex, line)) {
        exclude_list.push_back(line);
      }
      ex.close();
    }
  }


  std::ifstream fo(folders);
  if (fo.is_open()) {
    std::string line;
    while(getline(fo, line)) {
      std::vector<Video> v = get_files(line, ".mkv", exclude_list);
      videos.reserve(videos.size() + distance(v.begin(), v.end()));
      videos.insert(videos.end(), v.begin(), v.end());
    }
    fo.close();
  }
  else {
    throw std::string("Error loading folders!");
  }
}

void save_state(std::vector<Video>& videos, const std::string list_file) {
  std::ofstream ml(list_file);
  if (ml.is_open()) {
    for (const auto& v : videos) {
      ml << v.path.string() << '\n';
    }

    ml.close();
  }
  else {
    throw std::string("File cannot be saved!");
  }
}
