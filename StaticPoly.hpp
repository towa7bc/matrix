//
// Created by Michael Wittmann on 12/09/2020.
//

#ifndef MATRIX_STATICPOLY_HPP
#define MATRIX_STATICPOLY_HPP

#include <cmath>
#include <variant>
#include <vector>
//#include <memory_resource>

class Rectangle {
 public:
  Rectangle(double width, double height) : width_(width), height_(height) {}
  [[nodiscard]] double area() const { return width_ * height_; }
  void scale(std::size_t i) {
    width_ *= i;
    height_ *= i;
  }

 private:
  double width_{};
  double height_{};
};

class Triangle {
 public:
  Triangle(double width, double height) : width_(width), height_(height) {}
  [[nodiscard]] double area() const { return width_ * height_ / 2; }
  void scale(std::size_t i) { width_ *= i; }

 private:
  double width_{};
  double height_{};
};

class Circle {
 public:
  explicit Circle(double radius) : radius_(radius) {}
  [[nodiscard]] double area() const { return M_PI * std::pow(radius_, 2); }
  void scale(std::size_t i) { radius_ *= i; }

 private:
  double radius_{};
};

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T>
concept Shape = requires(T t) {
  { t.area() }
  ->std::same_as<double>;
  { t.scale(std::size_t{}) }
  ->std::same_as<void>;
};

int gggggg{100};
auto CallArea = [](Shape auto& obj) { return obj.area(); };
auto CallScale = [i = gggggg](Shape auto& obj) { obj.scale(i); };

template <Shape... Ss>
using var_t = std::variant<Ss...>;
template <Shape... Ss>
using vecVar_t = std::vector<var_t<Ss...>>;

template <Shape... Ss>
double GetArea(var_t<Ss...>& v) {
  return std::visit(overloaded{[](Ss& s) { return s.area(); }...}, v);
}

template <Shape... Ss>
std::vector<double> GetArea(vecVar_t<Ss...>& vec) {
  std::vector<double> retV;
  for (auto&& elem : vec) {
    retV.emplace_back(GetArea(elem));
  }
  return retV;
}

template <Shape... Ss>
void Scale(var_t<Ss...>& v, std::size_t factor) {
  std::visit(overloaded{[&](Ss& s) { s.scale(factor); }...}, v);
}

template <Shape... Ss>
void Scale(vecVar_t<Ss...>& vec, std::size_t factor) {
  for (auto&& elem : vec) {
    Scale(elem, factor);
  }
}

#endif  // MATRIX_STATICPOLY_HPP
