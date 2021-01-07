#ifndef H_ENGINE
#define H_ENGINE

#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Coord.hpp"
#include "Rect.hpp"

class Engine {
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_Event event;
  int width;
  int height;
  std::map<unsigned, TTF_Font*> font;
  std::map<std::string, SDL_Texture*> images;

  bool exit = false;

  Uint32 ticks = 0;
  Uint32 frame_time = 0;
  bool vsync = false;
  unsigned fps = 0;
  unsigned frame_counter = 0;
  float fps_timer = 0;
  float fps_cap = 0;
  float delay = 0;

  int mouse_position_x = 0;
  int mouse_position_y = 0;
  bool mouse_button_down = false;
  bool mouse_button_up = false;
  int mouse_scroll = 0;

  bool text_input = false;
  std::string text = "";
  int cursor = 0;
  int selection = 0;

  void draw_texture(SDL_Texture* texture, const Rect& body, const float& angle = 0,
    const SDL_Color& color = {255, 255, 255, 255}, const Rect& source = Rect(0, 0, 0, 0)) const;

public:
  // should be private or what
  const Uint8* keyboard_state = NULL;
  Uint32 mouse_state;

  // construct, destruct
  Engine(const int& width, const int& height, const std::string& title, const bool& vsync);
  ~Engine();

  // load fonts and images
  void load_font(const std::string& font_path, const unsigned& min_size = 8, const unsigned& max_size = 512, const unsigned& step = 8);
  void load_image(const std::string& image_path);
  void unload_image(const std::string& image_path);

  // methods for operations
  void render();

  void draw_text(const std::string& text, const Coord& position, const SDL_Color& color, const unsigned& size, const Rect& source = Rect(0, 0, 0, 0)) const;
  void draw_image(const std::string& filename, const Rect& body, const float& angle = 0,
    const SDL_Color& color = {255, 255, 255, 255}, const Rect& source = Rect(0, 0, 0, 0)) const;

  Rect size_text(const std::string& text, const unsigned& size) const;
  Rect size_image(const std::string& image) const;
  void update_inputs();
  void resize_window(const int& width, const int& height);

  // getters
  bool get_exit() const { return exit; }
  int get_width() const { return width; }
  int get_height() const { return height; }

  float get_ft() const { return (float)frame_time; }
  unsigned get_fps() const { return fps; }

  int get_mouse_pos_x() const { return mouse_position_x; }
  int get_mouse_pos_y() const { return mouse_position_y; }
  bool get_mouse_button_up() const { return mouse_button_up; }
  bool get_mouse_button_down() const { return mouse_button_down; }
  int get_mouse_scroll() const { return mouse_scroll; }

  std::string get_input_text() const { return text; }
  int get_input_cursor() const { return cursor; }
  int get_input_selection() const { return selection; }

  // setters
  void set_fps_cap(const float& set) { fps_cap = set; }
  void set_text_input(const bool& set);
};

#endif
