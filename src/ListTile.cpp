#include "ListTile.hpp"
#include <iostream>
ListTile::ListTile(const std::string& style, const Rect& body, const std::string& text,
  const unsigned& text_size, const SDL_Color& text_color, const Coord& cut_size)
  : style(style), cut_size(cut_size), body(body), text(text), text_size(text_size),
  text_color(text_color) {}

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
  if (mouse_hover) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(style, body, 0, {255, 255, 255, 100}, a_cut);
  }

  Rect a_text = engine.size_text(text, text_size);
  Coord place = Coord(body.x, body.y) + Coord(text_size / 2, (body.h / 2) - (a_text.h / 2));
  engine.draw_text(text, place, text_color, text_size);
}
