#include "Engine.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>

Engine::Engine(const int& width, const int& height, const std::string& title, const bool& vsync) : width(width), height(height), exit(false), vsync(vsync) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL initialization failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }
  else {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      std::cerr << "Window initialization failed! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else {
      if (vsync) { renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); }
      else { renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); }
      if (renderer == NULL) {
        std::cerr << "Renderer initialization failed! SDL_Error: " << SDL_GetError() << std::endl;
      }
    }
  }
  if (TTF_Init() < 0) {
    std::cerr << "TTF initialization failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }

  update_inputs();
  set_text_input(false);
}

Engine::~Engine() {
  for (auto& i : images) {
    SDL_DestroyTexture(i.second);
  }
  for (auto& f : font) {
    TTF_CloseFont(f.second);
  }
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Engine::render() {
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);

  if (fps_cap > 0) {
    Uint32 t_frame_time = SDL_GetTicks() - ticks;
    delay += (1000 / fps_cap) - (float)t_frame_time;

    if (delay > 0) {
      SDL_Delay((Uint32)delay);
      delay -= floor(delay);
    }
  }

  frame_time = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  //calculating fps
  fps_timer += get_ft();
  frame_counter++;

  if (fps_timer >= 1000) {
    fps_timer -= 1000;
    fps = frame_counter;
    frame_counter = 0;
  }
}

void Engine::load_font(const std::string& font_path, const unsigned& min_size, const unsigned& max_size, const unsigned& step) {
  for (auto& f : font) {
    TTF_CloseFont(f.second);
  }
  if ((((int)max_size - (int)min_size) / step) > 128) {
    std::cerr << "Engine::load_font: Warning, may experience performance issues due to overloaded font sizes!" << std::endl;
  }
  for (unsigned i = min_size; i <= max_size; i += step) {
    TTF_Font* temp_font = TTF_OpenFont(font_path.c_str(), i);
    if (temp_font == NULL) {
        std::cerr << "Font initialization failed at size: " << i << "!, SDL_Error: " << SDL_GetError() << std::endl;
    }
    else {
      font[i] = temp_font;
    }
  }
}

void Engine::load_image(const std::string& image_path) {
  SDL_Texture* texture = NULL;
  SDL_Surface* surface = IMG_Load(image_path.c_str());
  if (surface == NULL) {
    std::cerr << "Engine::load_image: load failed!, File: " + image_path << IMG_GetError() << std::endl;
  }
  else {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
  }
  if (texture == NULL) {
    std::cerr << "Engine::load_image: load failed!, File: " + image_path << IMG_GetError() << std::endl;
  }
  else {
    images.insert(std::make_pair(image_path, texture));
  }
}

void Engine::unload_image(const std::string& image_path) {
  SDL_DestroyTexture(images[image_path]);
  images.erase(image_path);
}

void Engine::draw_texture(SDL_Texture* texture, const Rect& body, const float& angle, const SDL_Color& color, const Rect& source) const {
  SDL_Rect rect;
  rect.x = round(body.x);
  rect.y = round(body.y);

  if ((body.w + body.h) <= 0) {
    SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
  }
  else {
    rect.w = round(body.w);
    rect.h = round(body.h);
  }

  SDL_Rect src;
  src.x = round(source.x);
  src.y = round(source.y);

  if ((source.w + source.h) <= 0) {
    SDL_QueryTexture(texture, 0, 0, &src.w, &src.h);
  }
  else {
    src.w = round(source.w);
    src.h = round(source.h);
  }

  SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(texture, color.a);

  if (SDL_RenderCopyEx(renderer, texture, &src, &rect, angle, NULL, SDL_FLIP_NONE) != 0) {
    std::cerr << "Texture copying failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }

  SDL_SetTextureColorMod(texture, 255, 255, 255);
  SDL_SetTextureAlphaMod(texture, 255);
}

void Engine::draw_text(const std::string& text, const Coord& position, const SDL_Color& color, const unsigned& size, const Rect& source) const {
  if (text.size() > 0) {
    SDL_Texture* texture = NULL;
    if (!font.count(size)) {
      std::cerr << "Engine::draw_text: invalid font size of " << size << std::endl;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font.at(size), text.c_str(), color);
    if (surface == NULL) {
      std::cerr << "Engine::draw_text: cannot create text!" << SDL_GetError() << std::endl;
    }
    else {
      texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
    }
    draw_texture(texture, Rect(position.x, position.y, source.w, source.h), 0, {255, 255, 255, 255}, source);
    SDL_DestroyTexture(texture);
  }
}

void Engine::draw_image(const std::string& filename, const Rect& body, const float& angle, const SDL_Color& color, const Rect& source) const {
  SDL_Texture* texture = NULL;
  try { texture = images.at(filename); }
  catch (const std::out_of_range& e) {
    std::cerr << "Engine::draw_image: image not loaded! " + filename << std::endl;
  }
  draw_texture(texture, body, angle, color, source);
}

Rect Engine::size_text(const std::string& text, const unsigned& size) const {
  int w, h;
  if(TTF_SizeText(font.at(size), text.c_str(), &w, &h) < 0) {
    std::cerr << "Failed to get text size!, SDL_Error: " << SDL_GetError() << std::endl;
    return Rect(0, 0, 0, 0);
  }
  return Rect(0, 0, w, h);
}

Rect Engine::size_image(const std::string& image) const {
  int w, h;
  if(SDL_QueryTexture(images.at(image), NULL, NULL, &w, &h) < 0) {
    std::cerr << "Failed to get image size!, SDL_Error: " << SDL_GetError() << std::endl;
    return Rect(0, 0, 0, 0);
  }
  return Rect(0, 0, w, h);
}

void Engine::update_inputs() {
  mouse_button_up = false;
  mouse_button_down = false;
  mouse_scroll = 0;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        exit = true;
        break;
      case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0) { mouse_scroll = 1; }
        if (event.wheel.y < 0) { mouse_scroll = -1; }
        break;
      case SDL_MOUSEBUTTONUP:
        mouse_button_up = true;
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouse_button_down = true;
        break;
      case SDL_KEYDOWN:
        if (text_input) {
          if (event.key.keysym.sym == SDLK_DELETE && text.size() > 0) {
            if (cursor == selection) {
              text.erase(cursor, 1);
            }
            else {
              text.erase(std::min(cursor, selection), abs(cursor - selection));
              cursor = std::min(cursor, selection);
            }
            selection = cursor;
          }
          if (event.key.keysym.sym == SDLK_BACKSPACE && text.size() > 0) {
            if ((cursor == selection) && (cursor > 0)) {
              text.erase(cursor - 1, 1);
              --cursor;
            }
            else {
              text.erase(std::min(cursor, selection), abs(cursor - selection));
              cursor = std::min(cursor, selection);
            }
            selection = cursor;
          }
          else if (event.key.keysym.sym == SDLK_LEFT) {
            if (cursor > 0) {
              --cursor;
            }
            if (!(SDL_GetModState() & KMOD_SHIFT)) { selection = cursor; }
          }
          else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (cursor < (int)text.size()) {
              ++cursor;
            }
            if (!(SDL_GetModState() & KMOD_SHIFT)) { selection = cursor; }
          }
          else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
            SDL_SetClipboardText(text.substr(std::min(cursor, selection), abs(cursor - selection)).c_str());
          }
          else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
            std::string paste = SDL_GetClipboardText();
            text = text.substr(0, std::min(cursor, selection)) + paste + text.substr(std::max(cursor, selection));
            cursor = std::min(cursor, selection);
            cursor += paste.size();
            selection = cursor;
          }
        }
        break;
      case SDL_TEXTINPUT:
        if (text_input) {
          if (!(SDL_GetModState() & KMOD_CTRL && (
            event.text.text[0] == 'c' ||
            event.text.text[0] == 'v' ||
            event.text.text[0] == 'C' ||
            event.text.text[0] == 'V'
          ))) {
            text = text.substr(0, std::min(cursor, selection)) + event.text.text + text.substr(std::max(cursor, selection));
            ++cursor;
            selection = cursor;
          }
        }
        break;
    }
  }
  keyboard_state = SDL_GetKeyboardState(NULL);
  if (mouse_button_up) {
    SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
  }
  else {
    mouse_state = SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
  }
}

void Engine::resize_window(const int& new_width, const int& new_height) {
  width = new_width;
  height = new_height;
  SDL_SetWindowSize(window, width, height);
}

void Engine::set_text_input(const bool& set) {
  if (text_input != set) {
    text_input = set;
    if (text_input) {
      SDL_StartTextInput();
    }
    else {
      SDL_StopTextInput();
    }
  }
}
