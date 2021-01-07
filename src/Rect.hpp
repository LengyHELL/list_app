#ifndef H_RECT
#define H_RECT

struct Rect {
  Rect() {}
  Rect(const float& x, const float& y, const float& w, const float& h) : x(x), y(y), w(w), h(h) {}

  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;

  Rect copy(const float& x = 0, const float& y = 0, const float& w = 0, const float& h = 0) {
    return Rect(this->x + x, this->y + y, this->w + w, this->h + h);
  }

  friend std::ostream& operator<<(std::ostream& os, const Rect& out) {
    os << "(" << out.x << "," << out.y << "," << out.w << "," << out.h << ")";
    return os;
  }
};

#endif
