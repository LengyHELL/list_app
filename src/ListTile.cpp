#include "ListTile.hpp"

ListTile::ListTile(const std::string& style, const Rect& body, const std::string& text,
  const unsigned& text_size, const SDL_Color& text_color, const bool& on_mouse_down, const Coord& cut_size)
  : style(style), cut_size(cut_size), body(body), text(text), text_size(text_size),
  text_color(text_color), on_mouse_down(on_mouse_down) {}

void ListTile::draw(const Engine& engine) {
  style_cut.clear();
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y), cut_size));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + cut_size.x, body.y), Coord(body.w - 2 * cut_size.x, cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y), cut_size));

  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y + cut_size.y), Coord(cut_size.x, body.h - 2 * cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y + cut_size.y), Coord(cut_size.x, body.h - 2 * cut_size.y)));

  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y + (body.h - cut_size.y)), cut_size));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + cut_size.x, body.y + (body.h - cut_size.y)), Coord(body.w - 2 * cut_size.x, cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y + (body.h - cut_size.y)), cut_size));

  Rect a_cut(8 * cut_size.x, 0, cut_size.x, cut_size.y);
  engine.draw_image(style, body, 0, {255, 255, 255, 255}, a_cut);

  unsigned it = 0;
  for (const auto& c : style_cut) {
    a_cut.x = it * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(style, Rect(c.first.x, c.first.y, c.second.x, c.second.y), 0, {255, 255, 255, 255}, a_cut);
    ++it;
  }

  if (selected) {
    a_cut.x = 10 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(style, body, 0, {255, 255, 255, 100}, a_cut);
  }
  else if (mouse_hover) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(style, body, 0, {255, 255, 255, 100}, a_cut);
  }

  Rect a_text = engine.size_text(text, text_size);
  Coord place = Coord(body.x, body.y) + Coord(text_size / 2, (body.h / 2) - (a_text.h / 2));
  engine.draw_text(text, place, text_color, text_size);
}

void ListTile::update(const Engine& engine) {
  bool x_in_range = (engine.get_mouse_pos_x() >= body.x) && (engine.get_mouse_pos_x() <= (body.x + body.w));
  bool y_in_range = (engine.get_mouse_pos_y() >= body.y) && (engine.get_mouse_pos_y() <= (body.y + body.h));
  mouse_hover = x_in_range && y_in_range;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
    mouse_hold = false;
    timer = 250;
  }
  if (!mouse_hover && mouse_left) {
    click_lock = true;
  }
  if (mouse_left && mouse_hover && selected) {
    timer -= engine.get_ft();
    if (timer < 0) {
      timer = 0;
    }
  }
  if (timer <= 0) { mouse_hold = true; }

  if (!mouse_hold) {
    if (on_mouse_down) {
      mouse_click = mouse_hover && mouse_left && !click_lock && engine.get_mouse_button_down();
    }
    else {
      mouse_click = mouse_hover && mouse_left && !click_lock && engine.get_mouse_button_up();
    }
  }
}
