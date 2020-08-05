//
// Created by Michael Wittmann on 04/08/2020.
//

#ifndef MATRIX_VEC_HPP
#define MATRIX_VEC_HPP

#include <cassert>
#include <utility>
#include <vector>

// template <typename T>
// concept ArithmeticNoBool = std::is_arithmetic_v<T> && !std::is_same_v<bool,
// T>;

template <typename E>
class VecExpression {
 public:
  double operator()(size_t i) const {
    // Delegation to the actual expression type. This avoids dynamic
    // polymorphism (a.k.a. virtual functions in C++)
    return static_cast<const E&>(*this)(i);
  }
  [[nodiscard]] size_t size() const {
    return static_cast<const E&>(*this).size();
  }
};

class Vector : public VecExpression<Vector> {
 private:
  std::vector<double> elems_;

 public:
  double operator()(size_t i) const { return elems_[i]; }
  double& operator()(size_t i) { return elems_[i]; }
  [[nodiscard]] size_t size() const { return elems_.size(); }

  explicit Vector(size_t n) : elems_(n) {}

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

  double operator()(size_t i) const { return u_(i) + v_(i); }
  [[nodiscard]] size_t size() const { return v_.size(); }
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

  double operator()(size_t i) const { return u_(i) - v_(i); }
  [[nodiscard]] size_t size() const { return v_.size(); }
};

template <typename E1, typename E2>
VecDiff<E1, E2> operator-(const VecExpression<E1>& u,
                          const VecExpression<E2>& v) {
  return VecDiff<E1, E2>(*static_cast<const E1*>(&u),
                         *static_cast<const E2*>(&v));
}

#endif  // MATRIX_VEC_HPP
