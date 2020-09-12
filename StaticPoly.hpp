//
// Created by Michael Wittmann on 12/09/2020.
//

#ifndef MATRIX_STATICPOLY_HPP
#define MATRIX_STATICPOLY_HPP

#include <cmath>
#include <variant>
#include <vector>

class Rectangle {
 public:
  Rectangle(double width, double height) : width_(width), height_(height) {}
  [[nodiscard]] double area() const { return width_ * height_; }
  bool isDay(int i) {
    ++i;
    return true;
  }

 private:
  double width_{};
  double height_{};
};

class Triangle {
 public:
  Triangle(double width, double height) : width_(width), height_(height) {}
  [[nodiscard]] double area() const { return width_ * height_ / 2; }
  bool isDay(int i) { return false; }

 private:
  double width_{};
  double height_{};
};

class Circle {
 public:
  explicit Circle(double radius) : radius_(radius) {}
  [[nodiscard]] double area() const { return M_PI * std::pow(radius_, 2); }
  bool isDay(int i) { return false; }

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
  { t.isDay(int{}) }
  ->std::same_as<bool>;
};

int gggggg{10};
auto CallArea = [](Shape auto& obj) { return obj.area(); };
auto CallIsDay = [i = gggggg](Shape auto& obj) { return obj.isDay(i); };

template <Shape... S>
using var = std::variant<S...>;
template <Shape... S>
using vecVar = std::vector<var<S...>>;

template <Shape... S>
double GetArea(var<S...> v) {
  auto ret = std::visit(overloaded{[](S s) { return s.area(); }...}, v);
  return ret;
}

template <Shape... S>
std::vector<double> GetArea(vecVar<S...>& v) {
  std::vector<double> retV;
  for (auto&& elem : v) {
    retV.emplace_back(GetArea(elem));
  }
  return retV;
}

#endif  // MATRIX_STATICPOLY_HPP
