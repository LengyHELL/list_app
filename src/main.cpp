#include <filesystem>
#include <vector>
#include <string>
#include <math.h>

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

  Frame base_frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height()));

  std::vector<Video> videos = get_files("./test", ".mkv");
  std::vector<ListTile> rows;

  float side_gap = 10;
  float offset = 30;
  float tile_height = 25;
  float window_size = engine.get_height() - 2 * side_gap;

  bool mouse_hold = false;
  bool mouse_set = false;
  float mouse_start = 0;
  float drag = 0;

  float auto_scroll_dist = 60;
  float scroll_size = 15;

  float selection_start = -1;
  float selection_end = -1;

  float pos = 0;

  for (const auto& v : videos) {
    rows.push_back(ListTile("img/basic_style.png", Rect(side_gap, 0, engine.get_width() - 2 * side_gap, tile_height), v.name, 16, {0, 0, 0, 0}));
  }

  float length = offset * (rows.size() - 1) + tile_height;

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

        int min, max;
        if (selection_start < selection_end) {
          min = selection_start;
          max = selection_end;
        }
        else {
          max = selection_start;
          min = selection_end;
        }
        mouse_start = rows[min].body.y - engine.get_mouse_pos_y();
      }

      drag = (pos + engine.get_mouse_pos_y()) - mouse_start;

      if (engine.get_mouse_pos_y() < auto_scroll_dist) {
        pos -= (auto_scroll_dist - engine.get_mouse_pos_y()) / 2;
        if (pos < 0) { pos = 0; }
      }
      if (engine.get_mouse_pos_y() > (engine.get_height() - auto_scroll_dist)) {
        pos += (engine.get_mouse_pos_y() - (engine.get_height() - auto_scroll_dist)) / 2;
        if (pos > length - window_size) { pos = length - window_size; }
      }

      /*if (rows[array_pos].button_frame.body.y < side_gap) {
        rows[array_pos].button_frame.body.y = side_gap;
      }
      if (rows[array_pos].button_frame.body.y > (engine.get_height() - (side_gap + tile_height))) {
        rows[array_pos].button_frame.body.y = engine.get_height() - (side_gap + tile_height);
      }*/

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
        if (rows[j].selected) {
          rows[j].body.y = engine.get_mouse_pos_y() + mouse_start + (j - min) * offset;
        }
      }

      while ((((rows[max].body.y + tile_height) - rows[max + 1].body.y) > (tile_height / 1.5)) && (max < ((int)rows.size() - 1))) {
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

      while ((((rows[min - 1].body.y + tile_height) - rows[min].body.y) > (tile_height / 1.5)) && (min > 0)) {
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
  return 0;
}


/*
float fit_pos = (side_gap + i * offset) - pos;
int array_pos = i;

if (rows[i].selected) {

  if ((rows[i].mouse_hover && (engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT))) || click_lock) {
    if (!mouse_hold) {
      mouse_hold = true;
      mouse_start = fit_pos - engine.get_mouse_pos_y();
      drag_start = engine.get_mouse_pos_y();
    }

    drag = abs(drag_start - engine.get_mouse_pos_y());

    if (drag > 5) {
      click_lock = true;
    }

    if (click_lock) {
      rows[array_pos].button_frame.body.y = engine.get_mouse_pos_y() + mouse_start;

      if (rows[array_pos].button_frame.body.y < auto_scroll_dist) {
        pos -= (auto_scroll_dist - rows[array_pos].button_frame.body.y) / 2;
        if (pos < 0) { pos = 0; }
      }
      if (rows[array_pos].button_frame.body.y > (engine.get_height() - auto_scroll_dist)) {
        pos += (rows[array_pos].button_frame.body.y - (engine.get_height() - auto_scroll_dist)) / 2;
        if (pos > length - window_size) { pos = length - window_size; }
      }

      if (rows[array_pos].button_frame.body.y < side_gap) {
        rows[array_pos].button_frame.body.y = side_gap;
      }
      if (rows[array_pos].button_frame.body.y > (engine.get_height() - (side_gap + tile_height))) {
        rows[array_pos].button_frame.body.y = engine.get_height() - (side_gap + tile_height);
      }

      if (array_pos == selection_start) {
        while ((fit_pos - engine.get_mouse_pos_y() > 0) && (selection_start > 0) && (selection_end > 0)) {
          float min, max;
          if (selection_start < selection_end) {
            min = selection_start;
            max = selection_end;
          }
          else {
            max = selection_start;
            min = selection_end;
          }

          for (int j = min; j <= max; ++j) {
            TextButton temp = rows[j];
            rows[j] = rows[j - 1];
            rows[j - 1] = temp;
          }

          selection_start -= 1;
          selection_end -= 1;
          fit_pos = (side_gap + array_pos * offset) - pos;
        }
      }

      if (array_pos == selection_end) {
        while ((engine.get_mouse_pos_y() - fit_pos > tile_height) && (selection_start < ((int)rows.size() - 1)) && (selection_end < ((int)rows.size() - 1))) {
          float min, max;
          if (selection_start < selection_end) {
            min = selection_start;
            max = selection_end;
          }
          else {
            max = selection_start;
            min = selection_end;
          }

          for (int j = max; j <= min; --j) {
            TextButton temp = rows[j];
            rows[j] = rows[j + 1];
            rows[j + 1] = temp;
          }

          selection_start += 1;
          selection_end += 1;
          fit_pos = (side_gap + array_pos * offset) - pos;
        }
      }
    }
    else {
      rows[i].update(engine);
    }
  }
  if (engine.mouse_state != SDL_BUTTON(SDL_BUTTON_LEFT)) {
    rows[i].button_frame.body.y = fit_pos;
    rows[i].update(engine);
    click_lock = false;
    mouse_hold = false;
  }
}
else {
  rows[i].button_frame.body.y = fit_pos;
  rows[i].update(engine);
}

if (rows[array_pos].mouse_click && rows[array_pos].selected && !click_lock && !mouse_hold) {
  if (engine.keyboard_state[SDL_SCANCODE_LSHIFT]) {
    selection_end = array_pos;
    int min, max;

    if (selection_start < selection_end) {
      min = selection_start;
      max = selection_end;
    }
    else {
      max = selection_start;
      min = selection_end;
    }

    for (int j = min + 1; j < max; ++j) {
      rows[j].selected = true;
    }
  }
  else {
    for (int j = 0; j < (int)rows.size(); ++j) {
      if (j != array_pos) { rows[j].selected = false; }
    }
    selection_start = array_pos;
    selection_end = array_pos;
  }
}
*/
