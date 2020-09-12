//
// Created by Michael Wittmann on 12/09/2020.
//

#ifndef MATRIX_STATICPOLY_HPP
#define MATRIX_STATICPOLY_HPP

#include <algorithm>
#include <cmath>
#include <memory_resource>
#include <variant>
#include <vector>

class Rectangle {
 public:
  constexpr Rectangle(double width, double height)
      : width_(width), height_(height) {}
  [[nodiscard]] constexpr double area() const { return width_ * height_; }
  constexpr void scale(std::size_t i) {
    width_ *= i;
    height_ *= i;
  }

 private:
  double width_{};
  double height_{};
};

class Triangle {
 public:
  constexpr Triangle(double width, double height)
      : width_(width), height_(height) {}
  [[nodiscard]] constexpr double area() const { return width_ * height_ / 2; }
  constexpr void scale(std::size_t i) { width_ *= i; }

 private:
  double width_{};
  double height_{};
};

class Circle {
 public:
  explicit constexpr Circle(double radius) : radius_(radius) {}
  [[nodiscard]] constexpr double area() const {
    return M_PI * std::pow(radius_, 2);
  }
  constexpr void scale(std::size_t i) { radius_ *= i; }

 private:
  double radius_{};
};

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename S>
concept Shape = requires(S s) {
  { s.area() }
  ->std::same_as<double>;
  { s.scale(std::size_t{}) }
  ->std::same_as<void>;
};

template <Shape... Ss>
using var_t = std::variant<Ss...>;
template <Shape... Ss>
using vecVar_t = std::pmr::vector<var_t<Ss...>>;

template <Shape... Ss>
constexpr std::pmr::vector<double> GetArea(vecVar_t<Ss...>& vec) {
  std::pmr::vector<double> retV;
  std::ranges::transform(vec, std::back_inserter(retV), [&](var_t<Ss...>& v) {
    return std::visit(overloaded{[&](Ss& s) { return s.area(); }...}, v);
  });
  return retV;
}

template <Shape... Ss>
constexpr void Scale(vecVar_t<Ss...>& vec, std::size_t factor) {
  std::ranges::for_each(vec, [&](var_t<Ss...>& v) {
    std::visit(overloaded{[&](Ss& s) { s.scale(factor); }...}, v);
  });
}

#endif  // MATRIX_STATICPOLY_HPP
