#include "ImageButton.hpp"

ImageButton::ImageButton(const std::string& style, const Rect& body,
  const std::string& image, const bool& on_mouse_down, const Rect& source_rect, const Coord& cut_size)
  : button_frame(Frame(style, body, cut_size)), body(body),
  image(image), on_mouse_down(on_mouse_down), source_rect(source_rect) {}

void ImageButton::draw(const Engine& engine) {
  const Coord cut_size = button_frame.get_cut_size();
  const Rect body = button_frame.get_body();
  Rect a_cut(0, 0, cut_size.x, cut_size.y);

  button_frame.draw(engine);

  if (mouse_click) {
    a_cut.x = 10 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body, 0, {255, 255, 255, 100}, a_cut);
  }
  else if (mouse_hover) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body, 0, {255, 255, 255, 100}, a_cut);
  }

  Rect a_image = engine.size_image(image);
  if ((source_rect.w + source_rect.h) > 0) {
    a_image = source_rect;
  }
  float ratio = (body.h * 0.5) / a_image.h;
  a_image.h *= ratio;
  a_image.w *= ratio;
  Coord place = Coord(body.x, body.y) + (Coord(body.w, body.h) / 2);
  place -= Coord(a_image.w, a_image.h) / 2;
  engine.draw_image(image, Rect(place.x, place.y, a_image.w, a_image.h), 0, {255, 255, 255, 255}, source_rect);
}

void ImageButton::update(const Engine& engine) {
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

  if (on_mouse_down) {
    selected = mouse_click && engine.get_mouse_button_down();
  }
  else {
    selected = mouse_click && engine.get_mouse_button_up();
  }
}
