#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP

#include <concepts>
#include <exception>
#include <future>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace libMatrix::inline v1 {

class BadIndexException final : public std::exception {
 public:
  explicit BadIndexException(std::string what) : what_(std::move(what)) {}
  [[nodiscard]] const char* what() const noexcept override {
    return what_.data();
  }

 private:
  std::string what_;
};

class BadDimensionException final : public std::exception {
 public:
  explicit BadDimensionException(std::string what) : what_(std::move(what)) {}
  [[nodiscard]] const char* what() const noexcept override {
    return what_.data();
  }

 private:
  std::string what_;
};

using uint = unsigned int;

template <typename Vec>
class IVectorEqualityComparable {
 private:
  /// vector equality
  friend bool operator==(Vec vector1, Vec vector2) {
    return equal_vectors(std::move(vector1), std::move(vector2));
  }

  /// vector inequality
  friend bool operator!=(Vec vector1, Vec vector2) {
    return !(std::move(vector1) == std::move(vector2));
  }
};

template <typename Vec, typename Scalar>
class IVectorArithmeticOperations {
 private:
  /// vector addition
  friend Vec operator+(Vec vector1, Vec vector2) {
    return add_vectors(std::move(vector1), std::move(vector2));
  }

  /// vector subtraction
  friend Vec operator-(Vec vector1, Vec vector2) {
    return subtract_vectors(std::move(vector1), std::move(vector2));
  }

  /// scalar multiplication
  friend Scalar operator*(Vec vector1, Vec vector2) {
    return scalar_product(std::move(vector1), std::move(vector2));
  }

  /// multiply a scalar value with a vector
  friend Vec operator*(Scalar scalarValue, Vec vector) {
    return multiply_scalar_vector(scalarValue, std::move(vector));
  }

  /// multiply a vector with a scalar value
  friend Vec operator*(Vec vector, Scalar scalarValue) {
    return multiply_vector_scalar(std::move(vector), scalarValue);
  }

  /// calculate the cross product of two 3d vectors
  friend Vec cross(Vec v1, Vec v2) {
    return calc_cross(std::move(v1), std::move(v2));
  }
};

template <typename Vec>
class IVectorGenericOperations {
 private:
  /// swap vectors
  friend void swap(Vec& v1, Vec& v2) noexcept { v1.swap(v2); }

  /// print a complete vector
  friend std::ostream& operator<<(std::ostream& out, Vec v) {
    return print_vector(out, std::move(v));
  }
};

template <typename T>
concept ArithmeticNoBool = std::is_arithmetic_v<T> && !std::is_same_v<bool, T>;

template <ArithmeticNoBool T>
class Vector : private IVectorEqualityComparable<Vector<T>>,
               private IVectorArithmeticOperations<Vector<T>, T>,
               private IVectorGenericOperations<Vector<T>> {
 private:
  uint rows_{0};
  std::vector<T> data_;

 public:
  explicit Vector(uint rows) : rows_(rows) {
    if (rows == 0) {
      throw BadIndexException("Vector constructor has 0 size");
    }
    data_.resize(rows);
  }

  T& operator()(uint row) {
    if (row >= rows_) {
      throw BadIndexException("Vector constructor has 0 size");
    }
    return data_[row];
  }

  T operator()(uint row) const {
    if (row >= rows_) {
      throw BadIndexException("Vector constructor has 0 size");
    }
    return data_[row];
  }

  [[nodiscard]] int size() const { return rows_; }

  void swap(Vector& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
  }
};

/// #region helper functions

template <ArithmeticNoBool T>
std::ostream& print_vector(std::ostream& out, const Vector<T>& v) {
  for (auto index{0}; index < v.size(); ++index) {
    out << '|' << ' ' << v(index) << ' ' << '|' << '\n';
  }
  return out;
}

template <ArithmeticNoBool T>
Vector<T> calc_cross(const Vector<T>& v1, const Vector<T>& v2) {
  if (v1.size() != 3 || v2.size() != 3) {
    throw BadDimensionException(
        "Vector cross product is only defined in 3 dimensions.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(3);
    result(0) = v1(1) * v2(2) - v1(2) * v2(1);
    result(1) = -(v1(0) * v2(2) - v1(2) * v2(0));
    result(2) = v1(0) * v2(1) - v1(1) * v2(0);
    return result;
  });
  return resultFuture.get();
}

template <ArithmeticNoBool T>
Vector<T> multiply_scalar_vector(T scalarValue, const Vector<T>& vector) {
  Vector<T> result(vector.size());
  for (auto i = 0; i < vector.size(); ++i) {
    result(i) = scalarValue * vector(i);
  }
  return result;
}

template <ArithmeticNoBool T>
Vector<T> multiply_vector_scalar(const Vector<T>& vector, T scalarValue) {
  Vector<T> result(vector.size());
  result = scalarValue * vector;
  return result;
}

template <ArithmeticNoBool T>
T scalar_product(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    T result{0};
    for (auto i = 0; i < vector1.size(); ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <ArithmeticNoBool T>
Vector<T> subtract_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector1.size());
    for (auto i = 0; i < vector1.size(); ++i) {
      result(i) = vector1(i) - vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <ArithmeticNoBool T>
Vector<T> add_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector1.size());
    for (auto i = 0; i < vector1.size(); ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <ArithmeticNoBool T>
bool equal_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  bool returnValue = vector1.size() == vector2.size();
  for (int index{0}; index < vector1.size(); ++index) {
    returnValue &= vector1(index) == vector2(index);
  }
  return returnValue;
}

/// #endregion helper functions

}  // namespace libMatrix::inline v1

#endif  // MATRIX_VECTOR_HPP
