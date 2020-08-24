//
// Created by Michael Wittmann on 04/08/2020.
//

#ifndef MATRIX_VEC_HPP
#define MATRIX_VEC_HPP

#include <cassert>
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
  bool canPerform() { return true; }
  [[nodiscard]] bool canPerform() const { return true; }
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
concept Greetable = requires(T t, std::string const& name) {
  { t.greet(name) }
  ->std::same_as<void>;
};

class Greeter {
 public:
  template <Greetable T>
  Greeter(T data) : self_(std::make_shared<Model<T>>(std::move(data))) {}

  void greet(std::string const& name) const { self_->greet_(name); }

 private:
  struct Concept_t {
    virtual ~Concept_t() = default;
    virtual void greet_(std::string const&) const = 0;
  };

  template <Greetable T>
  struct Model final : public Concept_t {
    explicit Model(T data) : data_(std::move(data)) {}
    void greet_(std::string const& name) const override { data_.greet(name); }

   private:
    T data_;
  };

  std::shared_ptr<const Concept_t> self_;
};

class English {
 public:
  void greet(std::string const& name) const {
    std::cout << "Good day " << name << '\n';
  }
};

class French {
 public:
  void greet(std::string const& name) const {
    std::cout << "Bonjour " << name << '\n';
  }
};

#endif  // MATRIX_VEC_HPP
