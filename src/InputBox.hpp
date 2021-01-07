#ifndef H_INPUT_BOX
#define H_INPUT_BOX

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Frame.hpp"
#include <string>

class InputBox {
  Frame button_frame;

  Rect body = Rect(0, 0, 0, 0);

  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  bool on_mouse_down = false;
  bool click_lock = true;

  int selection = 0;
  
  int window_pos = 0;

public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  std::string text = "";

  // constructor, destructor
  InputBox() {}

  InputBox(const std::string& style, const Rect& body, const unsigned& text_size,
    const SDL_Color& text_color, const Coord& cut_size = Coord(3, 3));

  ~InputBox() {}

  //functions
  void draw(const Engine& engine);
  void update(Engine& engine);
};

#endif
