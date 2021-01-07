#ifndef H_TEXT_BOX
#define H_TEXT_BOX

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Frame.hpp"
#include <list>
#include <string>

class TextBox {
  Frame text_box_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string text = "";
  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  std::list<std::string> lines;

public:
  // variables
  bool mouse_hover = false;

  // constructor, destructor
  TextBox() {}

  TextBox(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const Coord& cut_size = Coord(3, 3));

  ~TextBox() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
