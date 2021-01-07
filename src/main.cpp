#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;


#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"


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

std::vector<Video> get_files(const std::string& dir, const std::string& ext) {
  std::vector<Video> vec;

  if (fs::is_directory(fs::path(dir))) {
    for (const auto& entry : fs::directory_iterator(dir)) {
      fs::path current = entry.path();
      if (!fs::is_directory(current) && (current.extension() == ".mkv")) {
        vec.push_back(Video(current.stem().string(), current));
      }
    }
  }
  return vec;
}

int main(int argc, char** argv) {

  Engine engine(640, 480, "Test", true);
  engine.set_fps_cap(60);

  engine.load_font("lhll.ttf");
  engine.load_image("img/star.png");
  engine.load_image("img/basic_style.png");

  Rect button_body(10, 10, 75, 25);

  Frame base_frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height()));
  TextButton tb1("img/basic_style.png", button_body.copy(20, 10), "Button", 16, {0, 0, 0, 0});
  ImageButton ib1("img/basic_style.png", button_body.copy(20, 40, 0, 25), "img/star.png");
  Picker p1("img/basic_style.png", button_body.copy(20, 95, 0, 25), 16, {0, 0, 0, 0}, 0.5, 23.1, 0.1, 10);
  TextBox tx1("img/basic_style.png", button_body.copy(20, 150, 75, 75), "nop", 16, {0, 0, 0, 0});
  InputBox in1("img/basic_style.png", button_body.copy(20, 255, 75, 0), 16, {0, 0, 0, 0});

  std::vector<Video> videos = get_files("./test", ".mkv");
  std::vector<TextButton> rows;
  float offset = 0;
  for (const auto& v : videos) {
    rows.push_back(TextButton("img/basic_style.png", Rect(10, 10 + offset, engine.get_width() - 20, 25), v.name, 16, {0, 0, 0, 0}));
    offset += 30;
  }

  while(!engine.get_exit()) {
    engine.update_inputs();
    base_frame.update(engine);

    for (auto& r : rows) {
      r.button_frame.body.y += engine.get_mouse_scroll() * 10;
      r.update(engine);
    }

    base_frame.draw(engine);
    for (auto& r : rows) {
      r.draw(engine);
    }

    engine.render();
  }
  return 0;
}
