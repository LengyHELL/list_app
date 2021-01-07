#include "Picker.hpp"
#include <sstream>
#include <iomanip>

Picker::Picker(const std::string& style, const Rect& body, const unsigned& text_size,
  const SDL_Color& text_color, const float& min, const float& max, const float& step,
  const float& actual, const Coord& button_cut_size, const Coord& cut_size)
  : body(body), text_size(text_size), text_color(text_color), min(min), max(max), step(step), actual(actual) {
    picker_frame = Frame(style, Rect(body.x, body.y, body.w - (body.h / 2), body.h), cut_size);
    up = ImageButton(
      style,
      Rect(body.x + (body.w - (body.h / 2)), body.y, body.h / 2, body.h / 2),
      style,
      true,
      Rect(0, cut_size.y, button_cut_size.x, button_cut_size.y),
      cut_size
    );
    down = ImageButton(
      style,
      Rect(body.x + (body.w - (body.h / 2)), body.y + (body.h / 2), body.h / 2, body.h / 2),
      style,
      true,
      Rect(button_cut_size.x, cut_size.y, button_cut_size.x, button_cut_size.y),
      cut_size
    );
  }

void Picker::draw(const Engine& engine) {
  picker_frame.draw(engine);
  up.draw(engine);
  down.draw(engine);

  std::stringstream stream;
  stream << std::fixed << std::setprecision(1) << actual;
  std::string text = stream.str();

  const Rect f_body = picker_frame.get_body();
  Rect a_text = engine.size_text(text, text_size);
  Coord place = Coord(f_body.x, f_body.y) + (Coord(f_body.w, f_body.h) / 2);
  place -= Coord(a_text.w, a_text.h) / 2;

  engine.draw_text(text, place, text_color, text_size);
}

void Picker::update(const Engine& engine) {
  Coord rel_mouse_pos(engine.get_mouse_pos_x() - body.x, engine.get_mouse_pos_y() - body.y);
  bool in_range = (rel_mouse_pos > Coord(0, 0)) && (rel_mouse_pos < Coord(body.w, body.h));

  bool prev = up.mouse_click || down.mouse_click;
  up.update(engine);
  down.update(engine);
  bool curr = up.mouse_click || down.mouse_click;

  if (in_range) { actual += (float)engine.get_mouse_scroll() * step; }

  if (up.mouse_click && (timer <= 0)) { actual += step; }
  if (down.mouse_click && (timer <= 0)) { actual -= step; }

  if (prev && curr) {
    if (timer > 0) { timer -= engine.get_ft(); }
    else { timer = repeat_rate; }
  }
  else if (prev && !curr) { timer = 0; }
  else if (!prev && curr) { timer = repeat_timer; }

  if (actual > max) { actual = max; }
  if (actual < min) { actual = min; }

  picker_frame.update(engine);
}
