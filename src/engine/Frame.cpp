#include "Frame.hpp"

Frame::Frame(const std::string& style, const Rect& body, const Coord& cut_size)
  : style(style), body(body), cut_size(cut_size) {
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y), cut_size));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + cut_size.x, body.y), Coord(body.w - 2 * cut_size.x, cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y), cut_size));

  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y + cut_size.y), Coord(cut_size.x, body.h - 2 * cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y + cut_size.y), Coord(cut_size.x, body.h - 2 * cut_size.y)));

  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x, body.y + (body.h - cut_size.y)), cut_size));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + cut_size.x, body.y + (body.h - cut_size.y)), Coord(body.w - 2 * cut_size.x, cut_size.y)));
  style_cut.push_back(std::pair<Coord, Coord>(Coord(body.x + (body.w - cut_size.x), body.y + (body.h - cut_size.y)), cut_size));
}

void Frame::draw(const Engine& engine) {
  Rect a_cut(8 * cut_size.x, 0, cut_size.x, cut_size.y);
  engine.draw_image(style, body, 0, {255, 255, 255, 255}, a_cut);

  unsigned it = 0;
  for (const auto& c : style_cut) {
    a_cut.x = it * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(style, Rect(c.first.x, c.first.y, c.second.x, c.second.y), 0, {255, 255, 255, 255}, a_cut);
    ++it;
  }
}

void Frame::update(const Engine& engine) {
  bool x_in_range = (engine.get_mouse_pos_x() >= body.x) && (engine.get_mouse_pos_x() <= (body.x + body.w));
  bool y_in_range = (engine.get_mouse_pos_y() >= body.y) && (engine.get_mouse_pos_y() <= (body.y + body.h));
  mouse_hover = x_in_range && y_in_range;
}
