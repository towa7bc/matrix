//
// Created by Michael Wittmann on 04/08/2020.
//

#ifndef MATRIX_VEC_HPP
#define MATRIX_VEC_HPP

#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

///// Expression Templates
template <typename E>
class VecExpression {
 public:
  double operator()(std::size_t i) const {
    // Delegation to the actual expression type. This avoids dynamic
    // polymorphism (a.k.a. virtual functions in C++)
    return static_cast<const E&>(*this)(i);
  }
  [[nodiscard]] std::size_t size() const {
    return static_cast<const E&>(*this).size();
  }
};

class Vector : public VecExpression<Vector> {
 private:
  std::vector<double> elems_;

 public:
  double operator()(std::size_t i) const { return elems_[i]; }
  double& operator()(size_t i) { return elems_[i]; }
  [[nodiscard]] std::size_t size() const { return elems_.size(); }

  explicit Vector(std::size_t n) : elems_(n) {}

  // construct vector using initializer list
  Vector(std::initializer_list<double> init) : elems_(init) {}

  // A Vec can be constructed from any VecExpression, forcing its evaluation.
  template <typename E>
  Vector(const VecExpression<E>& expr) : elems_(expr.size()) {
    for (size_t i = 0; i != expr.size(); ++i) {
      elems_[i] = expr(i);
    }
  }
};

template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2>> {
 private:
  const E1& u_;
  const E2& v_;

 public:
  VecSum(const E1& u, const E2& v) : u_(u), v_(v) {
    assert(u.size() == v.size());
  }

  double operator()(std::size_t i) const { return u_(i) + v_(i); }
  [[nodiscard]] std::size_t size() const { return v_.size(); }
};

template <typename E1, typename E2>
VecSum<E1, E2> operator+(const VecExpression<E1>& u,
                         const VecExpression<E2>& v) {
  return VecSum<E1, E2>(*static_cast<const E1*>(&u),
                        *static_cast<const E2*>(&v));
}

template <typename E1, typename E2>
class VecDiff : public VecExpression<VecDiff<E1, E2>> {
 private:
  const E1& u_;
  const E2& v_;

 public:
  VecDiff(const E1& u, const E2& v) : u_(u), v_(v) {
    assert(u.size() == v.size());
  }

  double operator()(std::size_t i) const { return u_(i) - v_(i); }
  [[nodiscard]] std::size_t size() const { return v_.size(); }
};

template <typename E1, typename E2>
VecDiff<E1, E2> operator-(const VecExpression<E1>& u,
                          const VecExpression<E2>& v) {
  return VecDiff<E1, E2>(*static_cast<const E1*>(&u),
                         *static_cast<const E2*>(&v));
}

template <typename Derived>
class Base {
 public:
  bool canPerformAction() {
    return static_cast<Derived&>(*this).canPerformAction();
  }
  [[nodiscard]] bool canPerformAction() const {
    return static_cast<const Derived&>(*this).canPerformAction();
  }
};

class Derived1 : public Base<Derived1> {
  friend class Base<Derived1>;

 private:
  bool canPerformAction() { return true; }
  [[nodiscard]] bool canPerformAction() const { return true; }
};

class Derived2 : public Base<Derived2> {
  friend class Base<Derived2>;

 private:
  bool canPerformAction() { return false; }
  [[nodiscard]] bool canPerformAction() const { return false; }
};

template <typename Derived>
void print(const Base<Derived>& base) {
  printf(" %s perform the action.\n",
         base.canPerformAction() ? "can" : "cannot");
}

class BaseOld {
 public:
  virtual bool canPerformAction() = 0;
  virtual ~BaseOld() = default;
};

class DerivedOld : public BaseOld {
 public:
  bool canPerformAction() override { return true; }
};

///// static polymorphism with concepts
template <typename T>
concept BaseType = requires(T t) {
  { t.canPerformAction() }
  ->std::same_as<bool>;
};

auto CallCanPerformAction = [](BaseType auto& obj) {
  return obj.canPerformAction();
};

template <typename>
inline constexpr bool always_false_v = false;

class Concrete1 {
 public:
  bool canPerformAction() { return false; }
  [[nodiscard]] bool canPerformAction() const { return false; }
};

class Concrete2 {
 public:
  bool canPerformAction() { return true; }
  [[nodiscard]] bool canPerformAction() const { return true; }
};

class Concrete3 {
 public:
  bool canPerformAction() { return true; }
  [[nodiscard]] bool canPerformAction() const { return true; }
};

template <BaseType T>
bool canPerformActionGeneric(T const& t) {
  return t.canPerformAction();
}

template <BaseType T, BaseType S>
std::vector<bool> canPerformActionGeneric(
    std::vector<std::variant<T, S>> const& vec) {
  std::vector<bool> retVec;
  for (auto& item : vec) {
    auto ret = std::visit(
        [](auto&& arg) {
          using U = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<U, Concrete1>) {
            return arg.canPerformAction();
          } else if constexpr (std::is_same_v<U, Concrete2>) {
            return arg.canPerformAction();
          } else {
            static_assert(always_false_v<U>, "no good visitor!");
          }
        },
        item);
    retVec.push_back(ret);
  }
  return retVec;
}

/// Sean Parent 2017
template <typename T>
concept Drawable = requires(T t, std::ostream out, std::size_t position) {
  { draw(t, out, position) }
  ->std::same_as<void>;
};

template <typename T>
void draw(T const& t, std::ostream& out, std::size_t position) {
  out << std::string(position, ' ') << t << '\n';
}

class object_t {
 public:
  template <typename T>
  object_t(T data) : self_(std::make_shared<model<T>>(std::move(data))) {}

  friend void draw(object_t const& obj, std::ostream& out,
                   std::size_t position) {
    obj.self_->draw_(out, position);
  }

 private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual void draw_(std::ostream& out, std::size_t position) const = 0;
  };

  template <typename T>
  struct model final : public concept_t {
    explicit model(T data) : data_(std::move(data)) {}
    void draw_(std::ostream& out, std::size_t position) const override {
      draw(data_, out, position);
    }

   private:
    T data_;
  };

  std::shared_ptr<concept_t const> self_;
};

using document_t = std::vector<object_t>;

void draw(document_t const& doc, std::ostream& out, std::size_t position) {
  out << std::string(position, ' ') << "<document>" << '\n';
  for (auto&& item : doc) {
    draw(item, out, position + 2);
  }
  out << std::string(position, ' ') << "</document>" << '\n';
}

struct french {};

void draw(french const&, std::ostream& out, std::size_t position) {
  out << std::string(position, ' ') << "Bonjour!" << '\n';
}

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
  { t.isGay(int{}) }
  ->std::same_as<bool>;
};

int gggggg{10};
auto CallArea = [](Shape auto& obj) { return obj.area(); };
auto CallIsGay = [i = gggggg](Shape auto& obj) { return obj.isGay(i); };

class Rectangle {
 public:
  Rectangle(double width, double height) : width_(width), height_(height) {}
  [[nodiscard]] double area() const { return width_ * height_; }
  bool isGay(int i) {
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
  bool isGay(int i) { return false; }

 private:
  double width_{};
  double height_{};
};

class Circle {
 public:
  explicit Circle(double radius) : radius_(radius) {}
  [[nodiscard]] double area() const { return M_PI * std::pow(radius_, 2); }
  bool isGay(int i) { return false; }

 private:
  double radius_{};
};

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

#endif  // MATRIX_VEC_HPP
