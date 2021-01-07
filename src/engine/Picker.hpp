#ifndef H_PICKER
#define H_PICKER

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Frame.hpp"
#include "ImageButton.hpp"
#include <list>
#include <string>

class Picker {
  Frame picker_frame;
  ImageButton up;
  ImageButton down;

  Rect body = Rect(0, 0, 0, 0);

  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  float repeat_timer = 500;
  float repeat_rate = 100;

  float timer = 0;

public:
  // variables
  float min = 0;
  float max = 0;
  float step = 0;
  float actual = 0;

  // constructor, destructor
  Picker() {}

  Picker(const std::string& style, const Rect& body, const unsigned& text_size,
    const SDL_Color& text_color, const float& min, const float& max, const float& step,
    const float& actual, const Coord& button_cut_size = Coord(10, 10), const Coord& cut_size = Coord(3, 3));

  ~Picker() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
