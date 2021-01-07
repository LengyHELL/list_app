#ifndef H_TEXT_BUTTON
#define H_TEXT_BUTTON

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Frame.hpp"
#include <string>

class TextButton {
  Frame button_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string text = "";
  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  bool on_mouse_down = false;
  bool click_lock = true;

public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_click = false;

  // constructor, destructor
  TextButton() {}

  TextButton(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const bool& on_mouse_down = false, const Coord& cut_size = Coord(3, 3));

  ~TextButton() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
