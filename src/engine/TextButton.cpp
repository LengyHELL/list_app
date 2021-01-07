#include "TextButton.hpp"

TextButton::TextButton(const std::string& style, const Rect& body, const std::string& text,
  const unsigned& text_size, const SDL_Color& text_color, const bool& on_mouse_down, const Coord& cut_size)
  : button_frame(Frame(style, body, cut_size)), body(body), text(text), text_size(text_size),
  text_color(text_color), on_mouse_down(on_mouse_down) {}

void TextButton::draw(const Engine& engine) {
  const Coord cut_size = button_frame.get_cut_size();
  const Rect body = button_frame.get_body();
  Rect a_cut(0, 0, cut_size.x, cut_size.y);

  button_frame.draw(engine);

  if (selected) {
    a_cut.x = 10 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body, 0, {255, 255, 255, 100}, a_cut);
  }
  if (mouse_hover) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body, 0, {255, 255, 255, 100}, a_cut);
  }

  Rect a_text = engine.size_text(text, text_size);
  Coord place = Coord(body.x, body.y) + (Coord(text_size / 2, (body.h / 2) - (a_text.h / 2)));
  engine.draw_text(text, place, text_color, text_size);
}

void TextButton::update(const Engine& engine) {
  button_frame.update(engine);
  mouse_hover = button_frame.mouse_hover;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
  }
  if (!mouse_hover && mouse_left) {
    click_lock = true;
  }
  mouse_click = mouse_hover && mouse_left && !click_lock;

  if (mouse_click) {
    if (on_mouse_down && engine.get_mouse_button_down()) {
      selected = !selected;
    }
    if (!on_mouse_down && engine.get_mouse_button_up()) {
      selected = !selected;
    }
  }
}
