#ifndef H_IMAGE_BUTTON
#define H_IMAGE_BUTTON

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Frame.hpp"
#include <string>

class ImageButton {
  Frame button_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string image = "";

  bool on_mouse_down = false;
  bool click_lock = true;

  Rect source_rect = Rect(0, 0, 0, 0);

public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_click = false;

  // constructor, destructor
  ImageButton() {}

  ImageButton(const std::string& style, const Rect& body, const std::string& image,
    const bool& on_mouse_down = false, const Rect& source_rect = Rect(0, 0, 0, 0), const Coord& cut_size = Coord(3, 3));

  ~ImageButton() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
