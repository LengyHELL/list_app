#ifndef H_COORD
#define H_COORD

#include <iostream>
#include <string>

struct Coord {
  Coord() {}
  Coord(const Coord& other) : x(other.x), y(other.y) {}
  Coord(const float& x, const float& y) : x(x), y(y) {}
  Coord(const std::string& str);
  ~Coord() {}

  float x = 0;
  float y = 0;

  void operator=(const Coord& other);
  void operator=(const std::string& str);
  void operator=(const float& flt);
  bool operator==(const Coord& other) const;
  bool operator!=(const Coord& other) const;
  Coord operator+(const Coord& other) const;
  Coord operator-(const Coord& other) const;
  Coord operator+(const float& other) const;
  Coord operator-(const float& other) const;
  void operator+=(const Coord& other);
  void operator-=(const Coord& other);
  void operator+=(const float& other);
  void operator-=(const float& other);
  Coord operator*(const Coord& other) const;
  Coord operator/(const Coord& other) const;
  Coord operator*(const float& other) const;
  Coord operator/(const float& other) const;
  void operator*=(const Coord& other);
  void operator/=(const Coord& other);
  void operator*=(const float& other);
  void operator/=(const float& other);

  bool operator>(const Coord& other) const;
  bool operator<(const Coord& other) const;
  bool operator>=(const Coord& other) const;
  bool operator<=(const Coord& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Coord& out);

  float len() const;
  void unit();
  Coord rotate(const int& rotation) const;
  float distance(const Coord& other) const;
  float get_degree() const; //zero upwards, increases clockwise til 360
  float get_degree(const Coord& other) const;
  Coord project_to(const Coord& other) const;
};

#endif
