#ifndef H_LIST_TILE
#define H_LIST_TILE

#include "engine/Engine.hpp"
#include "engine/Coord.hpp"
#include "engine/Rect.hpp"
#include <string>
#include <list>

class ListTile {
  std::string style = "";
  std::list<std::pair<Coord, Coord>> style_cut;
  Coord cut_size = Coord(0, 0);

public:
  Rect body = Rect(0, 0, 0, 0);

  std::string text = "";

private:
  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

public:
  // variables
  bool click_lock = true;
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_hold = false;
  bool mouse_down = false;
  bool mouse_up = false;
  float prev_mouse_y = 0;

  // constructor, destructor
  ListTile() {}

  ListTile(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const Coord& cut_size = Coord(3, 3));

  ~ListTile() {}

  //functions
  void draw(const Engine& engine);
};

#endif
