#include "TextBox.hpp"
#include <iostream>

std::list<std::string> convert(const Engine& engine, std::string text, const unsigned& text_size, const unsigned& width) {
  std::list<std::string> list;
  std::string ws = " \t\n\v\f\r";
  int ls = 0;
  int rs = text.find_first_of(ws);
  if (rs < 0) { rs = text.size() - 1; }
  while (rs >= 0) {
    int next = text.find_first_of(ws, rs + 1);
    if (next < 0) { next = text.size() - 1; }
    Rect size = engine.size_text(text.substr(ls, (next - ls) + 1), text_size);

    if (size.w > width) {
      list.push_back(text.substr(ls, (rs - ls) + 1));
      ls = rs + 1;
      rs = text.find_first_of(ws, ls);
    }
    else if (next == (int)text.size() - 1) {
      list.push_back(text.substr(ls, (next - ls) + 1));
      rs = -1;
    }
    else {
      rs = next;
    }
  }
  return list;
}

TextBox::TextBox(const std::string& style, const Rect& body, const std::string& text,
  const unsigned& text_size, const SDL_Color& text_color, const Coord& cut_size)
  : text_box_frame(Frame(style, body, cut_size)), body(body), text(text), text_size(text_size),
  text_color(text_color) {}

void TextBox::draw(const Engine& engine) {
  const Coord cut_size = text_box_frame.get_cut_size();
  const Rect body = text_box_frame.get_body();

  text_box_frame.draw(engine);

  lines = convert(engine, text, text_size, body.w - 10);

  int it = 0;
  int padding = (int)(cut_size.len() * 1.5);
  for (const auto& l : lines) {
    Coord place(body.x + padding, (body.y + padding) + text_size * it);
    engine.draw_text(l, place, text_color, text_size);
    ++it;
  }
}

void TextBox::update(const Engine& engine) {
  text_box_frame.update(engine);
  mouse_hover = text_box_frame.mouse_hover;
}
