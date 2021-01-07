#include "InputBox.hpp"

InputBox::InputBox(const std::string& style, const Rect& body,
  const unsigned& text_size, const SDL_Color& text_color, const Coord& cut_size)
  : button_frame(Frame(style, body, cut_size)), body(body), text_size(text_size),
  text_color(text_color) {}

void InputBox::draw(const Engine& engine) {
  const Coord cut_size = button_frame.get_cut_size();
  const Rect body = button_frame.get_body();
  Rect a_cut(0, 0, cut_size.x, cut_size.y);

  button_frame.draw(engine);

  float padding = (int)(cut_size.len() * 1.5);
  Coord place(body.x + padding, body.y + padding);
  float c_width = text_size / 8;

  float window_size = body.w - 2 * padding;
  Rect t_rect = engine.size_text(text, text_size);
  if (t_rect.w < window_size) { window_size = t_rect.w; }

  Rect c_rect = engine.size_text(text.substr(0, engine.get_input_cursor()), text_size);
  Rect s_rect = engine.size_text(text.substr(0, selection), text_size);
  float cursor_pos = c_rect.w;
  float selection_pos = s_rect.w;
  float selection_size = abs(c_rect.w - engine.size_text(text.substr(0, engine.get_input_selection()), text_size).w);

  if (selection_pos < window_pos) {
    window_pos = selection_pos;
  }
  else if ((selection_pos + selection_size) > (window_pos + window_size)) {
    window_pos = (selection_pos + selection_size) - window_size;
  }

  if (cursor_pos < window_pos) {
    window_pos = cursor_pos;
  }
  else if (cursor_pos > (window_pos + window_size)) {
    window_pos = cursor_pos - window_size;
  }

  if (selection_size > window_size) { selection_size = window_size; }
  if (selection_pos < window_pos) { selection_pos = window_pos; }
  if ((window_pos + window_size) > t_rect.w) { window_pos = t_rect.w - window_size; }
  selection_pos -= window_pos;

  engine.draw_text(text, place, text_color, text_size, Rect(window_pos, 0, window_size, t_rect.h));

  if (selected) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), Rect(place.x + (cursor_pos - window_pos) - (c_width / 2), place.y, c_width, text_size), 0, {255, 255, 255, 255}, a_cut);
  }

  a_cut.x = 10 * cut_size.x;
  a_cut.y = 0;
  engine.draw_image(button_frame.get_style(), Rect(place.x + selection_pos, place.y, selection_size, text_size), 0, {255, 255, 255, 100}, a_cut);
}

void InputBox::update(Engine& engine) {
  button_frame.update(engine);
  mouse_hover = button_frame.mouse_hover;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
  }
  bool mouse_click = mouse_left && !click_lock && engine.get_mouse_button_down();
  if (mouse_click && mouse_hover) {
    selected = true;
    engine.set_text_input(true);
  }
  else if (mouse_click) {
    selected = false;
    engine.set_text_input(false);
  }

  if (selected) {
    text = engine.get_input_text();

    int s = engine.get_input_selection();
    int c = engine.get_input_cursor();
    if (s < c) { selection = s; }
    else { selection = c; }
  }
}
