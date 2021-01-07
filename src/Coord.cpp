#include "Coord.hpp"
#include <math.h>
#define M_PI 3.14159265358979323846

Coord::Coord(const std::string& str) {
  *this = str;
}

std::ostream& operator<<(std::ostream& os, const Coord& out) {
  os << "(" << out.x << "," << out.y << ")";
  return os;
}

void Coord::operator=(const Coord& other) {
  x = other.x;
  y = other.y;
}

void Coord::operator=(const std::string& str) {
  bool error = false;

  if (str.length() < 5) { error = true; }

  unsigned i = 1;

  std::string num = "";
  while((str[i] != ',') && !error) {
    num += str[i];
    i++;
    if (i == str.length()) { error = true; }
  }
  if (!error) { x = std::stof(num); }
  else { x = 0; }

  i++;
  num = "";
  while((str[i] != ')') && !error) {
    num += str[i];
    i++;
    if (i == str.length()) { error = true; }
  }

  if (!error) { y = std::stof(num); }
  else { y = 0; }
}

void Coord::operator=(const float& flt) {
  x = flt;
  y = flt;
}

bool Coord::operator==(const Coord& other) const {
  return (x == other.x) && (y == other.y);
}

bool Coord::operator!=(const Coord& other) const {
  return !(*this == other);
}

Coord Coord::operator+(const Coord& other) const {
  return Coord(x + other.x, y + other.y);
}

Coord Coord::operator-(const Coord& other) const {
  return Coord(x - other.x, y - other.y);
}

Coord Coord::operator+(const float& other) const {
  return Coord(x + other, y + other);
}

Coord Coord::operator-(const float& other) const {
  return Coord(x - other, y - other);
}

void Coord::operator+=(const Coord& other) {
  x += other.x;
  y += other.y;
}

void Coord::operator-=(const Coord& other) {
  x -= other.x;
  y -= other.y;
}

void Coord::operator+=(const float& other) {
  x += other;
  y += other;
}

void Coord::operator-=(const float& other) {
  x -= other;
  y -= other;
}

Coord Coord::operator*(const Coord& other) const {
  return Coord(x * other.x, y * other.y);
}

Coord Coord::operator/(const Coord& other) const {
  return Coord(x / other.x, y / other.y);
}

Coord Coord::operator*(const float& other) const {
  return Coord(x * other, y * other);
}

Coord Coord::operator/(const float& other) const {
  return Coord(x / other, y / other);
}

void Coord::operator*=(const Coord& other) {
  x *= other.x;
  y *= other.y;
}

void Coord::operator/=(const Coord& other) {
  x /= other.x;
  y /= other.y;
}

void Coord::operator*=(const float& other) {
  x *= other;
  y *= other;
}

void Coord::operator/=(const float& other) {
  x /= other;
  y /= other;
}

bool Coord::operator>(const Coord& other) const {
  return (x > other.x) && (y > other.y);
}

bool Coord::operator<(const Coord& other) const {
  return (x < other.x) && (y < other.y);
}

bool Coord::operator>=(const Coord& other) const {
  return (x >= other.x) && (y >= other.y);
}
bool Coord::operator<=(const Coord& other) const {
  return (x <= other.x) && (y <= other.y);
}

float Coord::len() const {
  return sqrt((x * x) + (y * y));
}

void Coord::unit() {
  float length = len();
  if (length > 0) {
    x /= length;
    y /= length;
  }
}

Coord Coord::rotate(const int& rotation) const {
  Coord ret;
  if (rotation == 0) { ret = Coord(x, y); }
  else if (rotation == 1) { ret = Coord(-y, x); }
  else if (rotation == 2) { ret = Coord(-x, -y); }
  else if (rotation == 3) { ret = Coord(y, -x); }
  return ret;
}

float Coord::distance(const Coord& other) const {
  Coord temp = *this - other;
  return temp.len();
}

float Coord::get_degree() const {
  if ((x == 0) && (y == 0)) {
    return 0;
  }
  else {
    float deg = abs((atan(x / -y) / M_PI) * 180);
    if ((x >= 0) && (y < 0)) { deg = deg; }
    else if ((x > 0) && (y >= 0)) { deg = 180 - deg; }
    else if ((x <= 0) && (y > 0)) { deg = 180 + deg; }
    else if ((x < 0) && (y <= 0)) { deg = 360 - deg; }
    return deg;
  }
}

float Coord::get_degree(const Coord& other) const {
  float dp = (other.x * x) + (other.y * y);
  float temp = len() * other.len();
  if (temp > 0) { return (acos(dp / temp) / M_PI) * 180; }
  else { return 0; }
}

Coord Coord::project_to(const Coord& other) const {
  float dp = (other.x * x) + (other.y * y);
  float len = other.len();
  if (len > 0) { return other * (dp / (len * len)); }
  else { return Coord(0, 0); }
}
