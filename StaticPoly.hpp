//
// Created by Michael Wittmann on 12/09/2020.
//

#ifndef MATRIX_STATICPOLY_HPP
#define MATRIX_STATICPOLY_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <memory_resource>
#include <numbers>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

class Rectangle {
 public:
  using allocator_t = std::pmr::polymorphic_allocator<>;
  Rectangle(double width, double height)
      : Rectangle(width, height, allocator_t{}) {}
  Rectangle(double width, double height, allocator_t alloc)
      : width_(width),
        height_(height),
        id_("Bigggggggggggggggggggggggg Rectangle", alloc) {}
  [[nodiscard]] constexpr double area() const { return width_ * height_; }
  constexpr void scale(double i) {
    width_ *= i;
    height_ *= i;
  }

  Rectangle(Rectangle const& other, allocator_t alloc)
      : width_(other.width_), height_(other.height_), id_(other.id_, alloc) {}

  Rectangle(Rectangle const&) = default;
  Rectangle(Rectangle&&) noexcept = default;

  Rectangle(Rectangle&& other, allocator_t alloc)
      : width_(other.width_),
        height_(other.height_),
        id_(std::move(other.id_), alloc) {}

  Rectangle& operator=(Rectangle const& rhs) = default;
  Rectangle& operator=(Rectangle&& rhs) noexcept = default;

  ~Rectangle() = default;

  [[nodiscard]] allocator_t get_allocator() const {
    return id_.get_allocator();
  }

  [[nodiscard]] std::pmr::string get_id() && { return std::move(id_); }
  [[nodiscard]] const std::pmr::string& get_id() const& { return id_; }

 private:
  double width_{};
  double height_{};
  std::pmr::string id_;
};

class Triangle {
 public:
  using allocator_t = std::pmr::polymorphic_allocator<>;
  Triangle(double width, double height)
      : Triangle(width, height, allocator_t{}) {}
  Triangle(double width, double height, allocator_t alloc)
      : width_(width),
        height_(height),
        id_("Bigggggggggggggggggggggggg Triangle", alloc) {}
  [[nodiscard]] constexpr double area() const { return width_ * height_ / 2; }
  constexpr void scale(double i) { width_ *= i; }

  Triangle(Triangle const& other, allocator_t alloc)
      : width_(other.width_), height_(other.height_), id_(other.id_, alloc) {}

  Triangle(Triangle const&) = default;
  Triangle(Triangle&&) noexcept = default;

  Triangle(Triangle&& other, allocator_t alloc)
      : width_(other.width_),
        height_(other.height_),
        id_(std::move(other.id_), alloc) {}

  Triangle& operator=(Triangle const& rhs) = default;
  Triangle& operator=(Triangle&& rhs) noexcept = default;

  ~Triangle() = default;

  [[nodiscard]] allocator_t get_allocator() const {
    return id_.get_allocator();
  }

  [[nodiscard]] std::pmr::string get_id() && { return std::move(id_); }
  [[nodiscard]] const std::pmr::string& get_id() const& { return id_; }

 private:
  double width_{};
  double height_{};
  std::pmr::string id_;
};

class Circle {
 public:
  using allocator_t = std::pmr::polymorphic_allocator<>;
  explicit Circle(double radius) : Circle(radius, allocator_t{}) {}
  Circle(double radius, allocator_t alloc)
      : radius_(radius), id_("Bigggggggggggggggggggggggg Circle", alloc) {}
  [[nodiscard]] constexpr double area() const {
    return std::numbers::pi * std::pow(radius_, 2);
  }
  constexpr void scale(double i) { radius_ *= i; }

  Circle(Circle const&) = default;
  Circle(Circle&&) noexcept = default;

  Circle(Circle&& other, allocator_t alloc)
      : radius_(other.radius_), id_(std::move(other.id_), alloc) {}

  Circle& operator=(Circle const& rhs) = default;
  Circle& operator=(Circle&& rhs) noexcept = default;

  ~Circle() = default;

  [[nodiscard]] allocator_t get_allocator() const {
    return id_.get_allocator();
  }
  [[nodiscard]] std::pmr::string get_id() && { return std::move(id_); }
  [[nodiscard]] const std::pmr::string& get_id() const& { return id_; }

 private:
  double radius_{};
  std::pmr::string id_;
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
  { s.scale(double{}) }
  ->std::same_as<void>;
};

template <Shape... Ss>
using var_t = std::variant<Ss...>;
template <Shape... Ss>
using vecVar_t = std::pmr::vector<var_t<Ss...>>;

template <Shape... Ss>
constexpr std::pmr::vector<double> GetArea(vecVar_t<Ss...>& vec) {
  std::array<std::uint8_t, 3004> buffer{};
  std::pmr::monotonic_buffer_resource mem_resource(buffer.data(),
                                                   buffer.size());
  std::pmr::vector<double> retV{&mem_resource};
  retV.reserve(buffer.size());
  std::ranges::transform(
      vec, std::back_inserter(retV), [&](var_t<Ss...>& v) -> double {
        return std::visit(
            overloaded{[&](Ss& s) -> double { return s.area(); }...}, v);
      });
  return retV;
}

template <Shape... Ss>
constexpr void Scale(vecVar_t<Ss...>& vec, double factor) {
  std::ranges::for_each(vec, [&](var_t<Ss...>& v) {
    std::visit(overloaded{[&](Ss& s) { s.scale(factor); }...}, v);
  });
}

template <Shape... Ss>
constexpr std::pmr::string Get_Id(var_t<Ss...>& v) {
  return std::visit(
      overloaded{[&](Ss& s) -> std::pmr::string { return s.get_id(); }...}, v);
}

#endif  // MATRIX_STATICPOLY_HPP
