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

template <typename T>
concept ArithmeticNoBool = std::is_arithmetic_v<T> && !std::is_same_v<bool, T>;

template <ArithmeticNoBool T>
class Vector {
 private:
  uint rows_{0};
  std::vector<T> data_;

 public:
  explicit Vector(uint /*rows*/);

  T& operator()(uint /*row*/);
  T operator()(uint /*row*/) const;

  [[nodiscard]] int size() const { return rows_; }

  /// vector equality
  inline friend bool operator==(Vector<T> vector1, Vector<T> vector2) {
    return equal_vectors(std::move(vector1), std::move(vector2));
  }

  /// vector inequality
  inline friend bool operator!=(Vector<T> vector1, Vector<T> vector2) {
    return !(std::move(vector1) == std::move(vector2));
  }

  /// vector addition
  inline friend Vector<T> operator+(Vector<T> vector1, Vector<T> vector2) {
    return add_vectors(std::move(vector1), std::move(vector2));
  }

  /// vector subtraction
  inline friend Vector<T> operator-(Vector<T> vector1, Vector<T> vector2) {
    return subtract_vectors(std::move(vector1), std::move(vector2));
  }

  /// scalar multiplication
  inline friend T operator*(Vector<T> vector1, Vector<T> vector2) {
    return scalar_product(std::move(vector1), std::move(vector2));
  }

  /// multiply a scalar value with a vector
  inline friend Vector<T> operator*(T scalarValue, Vector<T> vector) {
    return multiply_scalar_vector(scalarValue, std::move(vector));
  }

  /// multiply a vector with a scalar value
  inline friend Vector<T> operator*(Vector<T> vector, T scalarValue) {
    return multiply_vector_scalar(std::move(vector), scalarValue);
  }

  /// calculate the cross product of two 3d vectors
  inline friend Vector<T> cross(Vector<T> v1, Vector<T> v2) {
    return calc_cross(std::move(v1), std::move(v2));
  }

  /// print a complete vector
  inline friend std::ostream& operator<<(std::ostream& out, Vector<T> v) {
    return print_vector(out, std::move(v));
  }
};

/// #region class implementation

template <ArithmeticNoBool T>
Vector<T>::Vector(uint rows) : rows_(rows) {
  if (rows == 0) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  data_.resize(rows);
}

template <ArithmeticNoBool T>
inline T& Vector<T>::operator()(uint row) {
  if (row >= rows_) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  return data_[row];
}

template <ArithmeticNoBool T>
inline T Vector<T>::operator()(uint row) const {
  if (row >= rows_) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  return data_[row];
}

/// #endregion class implementation

/// #region helper functions

template <ArithmeticNoBool T>
inline std::ostream& print_vector(std::ostream& out, const Vector<T>& v) {
  for (auto index{0}; index < v.size(); ++index) {
    out << '|' << ' ' << v(index) << ' ' << '|' << '\n';
  }
  return out;
}

template <ArithmeticNoBool T>
inline Vector<T> calc_cross(const Vector<T>& v1, const Vector<T>& v2) {
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
inline Vector<T> multiply_scalar_vector(T scalarValue,
                                        const Vector<T>& vector) {
  Vector<T> result(vector.size());
  for (auto i = 0; i < vector.size(); ++i) {
    result(i) = scalarValue * vector(i);
  }
  return result;
}

template <ArithmeticNoBool T>
inline Vector<T> multiply_vector_scalar(const Vector<T>& vector,
                                        T scalarValue) {
  Vector<T> result(vector.size());
  result = scalarValue * vector;
  return result;
}

template <ArithmeticNoBool T>
inline T scalar_product(const Vector<T>& vector1, const Vector<T>& vector2) {
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
inline Vector<T> subtract_vectors(const Vector<T>& vector1,
                                  const Vector<T>& vector2) {
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
inline Vector<T> add_vectors(const Vector<T>& vector1,
                             const Vector<T>& vector2) {
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
inline bool equal_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
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
