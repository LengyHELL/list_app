#include <vector>
#include <string>
//#include <math.h>


#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Frame.hpp"
#include "engine/TextButton.hpp"

#include "ListTile.hpp"
#include "Video.hpp"
#include "Functions.hpp"


#include <iostream>



int main(int argc, char** argv) {
  std::vector<Video> videos;
  std::vector<ListTile> rows;

  std::cout << argv[1] << std::endl;

  load_state(videos, "folders.txt", "exclude.txt", "my_list.txt");

  Engine engine(1200, 675, "List App", true);
  engine.set_fps_cap(60);
  engine.load_font("lhll.ttf");
  engine.load_image("img/basic_style.png");


  bool saved = false;

  float control_bar_width = 200;
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

  Frame base_frame("img/basic_style.png", Rect(0, 0, engine.get_width() - control_bar_width, engine.get_height()));
  Frame controls_frame("img/basic_style.png", Rect(engine.get_width() - control_bar_width, 0, control_bar_width, engine.get_height()));
  TextButton save_button("img/basic_style.png", Rect(engine.get_width() - (control_bar_width - side_gap), side_gap, control_bar_width - 2 * side_gap, 25), "Save", 16, {0, 0, 0, 0});

  for (const auto& v : videos) {
    rows.push_back(ListTile("img/basic_style.png", Rect(side_gap, 0, (engine.get_width() - control_bar_width) - 2 * side_gap, tile_height), v.name, 16, {0, 0, 0, 0}));
  }

  float length = offset * (rows.size() - 1) + tile_height;

  bool delete_lock = true;

  while(!engine.get_exit()) {
    engine.update_inputs();
    base_frame.update(engine);
    controls_frame.update(engine);
    save_button.update(engine);

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

    if (save_button.selected) {
      save_button.selected = false;
      saved = true;
      save_state(videos, "my_list.txt");
    }


    // drawing
    base_frame.draw(engine);
    controls_frame.draw(engine);

    save_button.draw(engine);

    for (auto& r : rows) {
      if (!r.selected) { r.draw(engine); }
    }

    for (auto& r : rows) {
      if (r.selected) { r.draw(engine); }
    }

    engine.render();
  }

  if ((argc > 1) && saved) {
    system(argv[1]);
  }

  return 0;
}
