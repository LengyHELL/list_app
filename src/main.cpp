#include <filesystem>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <fstream>

namespace fs = std::filesystem;


#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Frame.hpp"

#include "ListTile.hpp"


#include <iostream>

struct Video {
  Video(const std::string& name, const std::string& path) : name(name), path(path) {}
  Video(const std::string& name, const fs::path& path) : name(name), path(path) {}

  std::string name;
  fs::path path;
};

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

int main(int argc, char** argv) {

  Engine engine(640, 480, "List App", true);
  engine.set_fps_cap(60);

  engine.load_font("lhll.ttf");
  engine.load_image("img/basic_style.png");

  Frame base_frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height()));

  std::vector<std::string> exclude;

  std::fstream file("my_list.txt");
  if (file.is_open()) {
    std::ofstream file2("exclude.txt");
    if (file2.is_open()) {
      std::string line;

      while(getline(file, line)) {
        file2 << line << '\n';
      }
      file2.close();
    }
    file.close();
  }

  file.open("exclude.txt");
  if (file.is_open()) {
    std::string line;
    while(getline(file, line)) {
      exclude.push_back(line);
    }
    file.close();
  }

  std::vector<Video> videos;

  file.open("folders.txt");
  if (file.is_open()) {
    std::string line;
    while(getline(file, line)) {
      std::vector<Video> v = get_files(line, ".mkv", exclude);
      videos.reserve(videos.size() + distance(v.begin(), v.end()));
      videos.insert(videos.end(), v.begin(), v.end());
    }
    file.close();
  }
  else {
    return 1;
  }

  std::vector<ListTile> rows;

  float side_gap = 10;
  float offset = 30;
  float tile_height = 25;
  float window_size = engine.get_height() - 2 * side_gap;

  bool mouse_hold = false;
  bool mouse_set = false;
  float mouse_start = 0;

  float auto_scroll_dist = 60;
  float scroll_size = 15;

  float selection_start = -1;
  float selection_end = -1;

  float pos = 0;

  for (const auto& v : videos) {
    rows.push_back(ListTile("img/basic_style.png", Rect(side_gap, 0, engine.get_width() - 2 * side_gap, tile_height), v.name, 16, {0, 0, 0, 0}));
  }

  float length = offset * (rows.size() - 1) + tile_height;

  bool delete_lock = true;

  while(!engine.get_exit()) {
    engine.update_inputs();
    base_frame.update(engine);

    pos -= engine.get_mouse_scroll() * scroll_size;
    if (pos > length - window_size) { pos = length - window_size; }
    if (pos < 0) { pos = 0; }

    mouse_hold = false;
    for (const auto& r : rows) {
      if (r.mouse_hold) {
        mouse_hold = true;
        break;
      }
    }

    for (unsigned i = 0; i < rows.size(); ++i) {
      rows[i].update(engine);

      if (rows[i].mouse_click) {
        if (engine.keyboard_state[SDL_SCANCODE_LSHIFT] && (selection_start >= 0)) {
          for (auto& r : rows) { r.selected = false; }

          selection_end = i;
          int min, max;

          if (selection_start < selection_end) {
            min = selection_start;
            max = selection_end;
          }
          else {
            max = selection_start;
            min = selection_end;
          }

          for (int j = min; j <= max; ++j) {
            rows[j].selected = true;
          }
        }
        else if (rows[i].selected && (selection_start == selection_end)) {
          rows[i].selected = false;
          selection_start = -1;
          selection_end = -1;
        }
        else {
          for (auto& r : rows) { r.selected = false; }
          rows[i].selected = true;
          selection_start = i;
          selection_end = i;
        }
      }

      if (!(mouse_hold && rows[i].selected)) {
        rows[i].body.y = (side_gap + i * offset) - pos;
      }
    }

    if (mouse_hold) {
      if (!mouse_set) {
        mouse_set = true;

        int min;
        if (selection_start < selection_end) { min = selection_start; }
        else { min = selection_end; }
        mouse_start = rows[min].body.y - engine.get_mouse_pos_y();
      }

      if (engine.get_mouse_pos_y() < auto_scroll_dist) {
        pos -= (auto_scroll_dist - engine.get_mouse_pos_y()) / 2;
        if (pos < 0) { pos = 0; }
      }
      if (engine.get_mouse_pos_y() > (engine.get_height() - auto_scroll_dist)) {
        pos += (engine.get_mouse_pos_y() - (engine.get_height() - auto_scroll_dist)) / 2;
        if (pos > length - window_size) { pos = length - window_size; }
      }

      int min, max;
      if (selection_start < selection_end) {
        min = selection_start;
        max = selection_end;
      }
      else {
        max = selection_start;
        min = selection_end;
      }

      float base = engine.get_mouse_pos_y() + mouse_start;

      if (base < side_gap) { base = side_gap; }
      if (base > (engine.get_height() - (side_gap + ((max - min) * offset) + tile_height))) {
        base = (engine.get_height() - (side_gap + ((max - min) * offset) + tile_height));
      }

      for (int j = min; j <= max; ++j) {
        if (rows[j].selected) {
          rows[j].body.y = base + (j - min) * offset;
        }
      }

      while ((((rows[max].body.y + tile_height) - rows[max + 1].body.y) > (tile_height / 2)) && (max < ((int)rows.size() - 1))) {
        for (int j = max; j >= min; --j) {
          ListTile temp = rows[j];
          rows[j] = rows[j + 1];
          rows[j + 1] = temp;

          Video temp_v = videos[j];
          videos[j] = videos[j + 1];
          videos[j + 1] = temp_v;
        }

        rows[min].body.y = (side_gap + min * offset) - pos;

        min++;
        max++;
        selection_start++;
        selection_end++;
      }

      while ((((rows[min - 1].body.y + tile_height) - rows[min].body.y) > (tile_height / 2)) && (min > 0)) {
        for (int j = min; j <= max; ++j) {
          ListTile temp = rows[j];
          rows[j] = rows[j - 1];
          rows[j - 1] = temp;

          Video temp_v = videos[j];
          videos[j] = videos[j - 1];
          videos[j - 1] = temp_v;
        }

        rows[max].body.y = (side_gap + max * offset) - pos;

        min--;
        max--;
        selection_start--;
        selection_end--;
      }
    }
    else {
      mouse_set = false;
    }

    if (engine.keyboard_state[SDL_SCANCODE_DELETE] && !delete_lock && (selection_start >= 0) && (selection_end >= 0)) {
      delete_lock = true;
      int min, max;

      if (selection_start < selection_end) {
        min = selection_start;
        max = selection_end;
      }
      else {
        max = selection_start;
        min = selection_end;
      }

      for (int j = max; j >= min; --j) {
        rows.erase(rows.begin() + j);
        videos.erase(videos.begin() + j);
      }

      length = offset * (rows.size() - 1) + tile_height;
      selection_start = -1;
      selection_end = -1;
    }

    if (!engine.keyboard_state[SDL_SCANCODE_DELETE]) { delete_lock = false; }


    // drawing
    base_frame.draw(engine);

    for (auto& r : rows) {
      if (!r.selected) { r.draw(engine); }
    }

    for (auto& r : rows) {
      if (r.selected) { r.draw(engine); }
    }

    engine.render();
  }

  std::fstream file2("my_list.txt");
  if (file2.is_open()) {
    file2.seekg(0, std::ios::end);

    for (const auto& v : videos) {
      file2 << v.path.string() << '\n';
    }

    file.close();
  }
  else {
    return 1;
  }
  return 0;
}
