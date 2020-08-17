//
// Created by Michael Wittmann on 04/08/2020.
//

#ifndef MATRIX_VEC_HPP
#define MATRIX_VEC_HPP

#include <cassert>
#include <cstdio>
#include <memory>
#include <utility>
#include <vector>

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
  virtual bool CanPerformAction() = 0;
  virtual ~BaseOld() = default;
};

class DerivedOld : BaseOld {
 public:
  bool CanPerformAction() override { return true; }
};

// template <typename T>
// concept ArithmeticNoBool = std::is_arithmetic_v<T> && !std::is_same_v<bool,
// T>;

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

#endif  // MATRIX_VEC_HPP
