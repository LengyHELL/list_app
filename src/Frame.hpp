#ifndef H_FRAME
#define H_FRAME

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include <list>
#include <string>

class Frame {
  std::string style = "";
  std::list<std::pair<Coord, Coord>> style_cut;

  Rect body = Rect(0, 0, 0, 0);

  Coord cut_size = Coord(0, 0);

public:
  // variables
  bool mouse_hover = false;

  // constructor, destructor
  Frame() {}
  Frame(const std::string& style, const Rect& body, const Coord& cut_size = Coord(3, 3));
  ~Frame() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);

  const std::string& get_style() const { return style; }
  const Rect& get_body() const { return body; }
  const Coord& get_cut_size() const { return cut_size; }
};

#endif
