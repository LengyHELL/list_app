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

  bool on_mouse_down = false;
  bool click_lock = true;

  float timer = 0;
public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_hold = false;
  bool mouse_click = false;

  // constructor, destructor
  ListTile() {}

  ListTile(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const bool& on_mouse_down = false, const Coord& cut_size = Coord(3, 3));

  ~ListTile() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
