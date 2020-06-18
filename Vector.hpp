#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

namespace libMatrix::inline v1 {

class BadIndexException : public std::exception {
 public:
  explicit BadIndexException(std::string what) : what_(std::move(what)) {}
  [[nodiscard]] const char* what() const noexcept override {
    return what_.data();
  }

 private:
  std::string what_;
};

class BadDimensionException : public std::exception {
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
class Vector {
 public:
  Vector() = delete;
  explicit Vector(uint /*rows*/);
  Vector(const Vector<T>& /*other*/);
  Vector<T>& operator=(const Vector<T>& /*other*/);
  Vector(Vector<T>&& /*other*/) noexcept;
  Vector<T>& operator=(Vector<T>&& /*other*/) noexcept;
  T& operator()(uint row);
  T operator()(uint row) const;
  ~Vector() noexcept = default;

  /// vector addition
  inline friend Vector<T> operator+(const Vector<T>& vector1,
                                    const Vector<T>& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  }

  /// vector addition movable
  inline friend Vector<T> operator+(Vector<T>&& vector1, Vector<T>&& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  }

  /// vector subtraction
  inline friend Vector<T> operator-(const Vector<T>& vector1,
                                    const Vector<T>& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) - vector2(i);
    }
    return result;
  }

  /// vector subtraction movable
  inline friend Vector<T> operator-(Vector<T>&& vector1, Vector<T>&& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) - vector2(i);
    }
    return result;
  }

  /// scalar multiplication
  inline friend T operator*(const Vector<T>& vector1,
                            const Vector<T>& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    T result{0};
    for (auto i = 0; i < vector1.rows_; ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  }

  /// scalar multiplication movable
  inline friend T operator*(Vector<T>&& vector1, Vector<T>&& vector2) {
    if (vector1.rows_ != vector2.rows_) {
      throw BadDimensionException("The vector dimensions have to be the same.");
    }
    T result{0};
    for (auto i = 0; i < vector1.rows_; ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  }

  /// multiply a vector with a scalar value
  inline friend Vector<T> operator*(T scalarValue, const Vector<T>& vector) {
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < vector.rows_; ++i) {
      result(i) = scalarValue * vector(i);
    }
    return result;
  }

  /// multiply a vector with a scalar value
  inline friend Vector<T> operator*(const Vector<T>& vector, T scalarValue) {
    Vector<T> result(vector.rows_);
    result = scalarValue * vector;
    return result;
  }

  /// multiply a vector with a scalar value movable
  inline friend Vector<T> operator*(Vector<T>&& vector, T scalarValue) {
    Vector<T> result(vector.rows_);
    result = scalarValue * vector;
    return result;
  }

  /// multiply a vector with a scalar value movable
  inline friend Vector<T> operator*(T scalarValue, Vector<T>&& vector) {
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < vector.rows_; ++i) {
      result(i) = scalarValue * vector(i);
    }
    return result;
  }

  /// calculate the cross product of two 3d vectors
  inline friend Vector<T> cross(const Vector<T>& v1, const Vector<T>& v2) {
    if (v1.rows_ != 3 || v2.rows_ != 3) {
      throw BadDimensionException(
          "Vector cross product is only defined in 3 dimensions.");
    }
    Vector<T> result(3);
    result(0) = v1(1) * v2(2) - v1(2) * v2(1);
    result(1) = -(v1(0) * v2(2) - v1(2) * v2(0));
    result(2) = v1(0) * v2(1) - v1(1) * v2(0);
    return result;
  }

  /// calculate the cross product of two 3d vectors movable
  inline friend Vector<T> cross(Vector<T>&& v1, Vector<T>&& v2) {
    if (v1.rows_ != 3 || v2.rows_ != 3) {
      throw BadDimensionException(
          "Vector cross product is only defined in 3 dimensions.");
    }
    Vector<T> result(3);
    result(0) = v1(1) * v2(2) - v1(2) * v2(1);
    result(1) = -(v1(0) * v2(2) - v1(2) * v2(0));
    result(2) = v1(0) * v2(1) - v1(1) * v2(0);
    return result;
  }

  /// print a complete vector
  inline friend std::ostream& operator<<(std::ostream& out,
                                         const Vector<T>& v) {
    for (auto index{0}; index < v.rows_; ++index) {
      out << '|' << v(index) << '|' << '\n';
    }
    return out;
  }

  /// print a complete vector movable
  inline friend std::ostream& operator<<(std::ostream& out, Vector<T>&& v) {
    for (auto index{0}; index < v.rows_; ++index) {
      out << '|' << v(index) << '|' << '\n';
    }
    return out;
  }

  [[nodiscard]] int size() const { return rows_; }

 private:
  uint rows_{0};
  std::unique_ptr<T[]> data_;
};

template <typename T>
Vector<T>::Vector(uint rows) : rows_(rows) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (rows == 0) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  data_ = std::make_unique<T[]>(rows);
}

template <typename T>
inline T& Vector<T>::operator()(uint row) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (row >= rows_) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  return data_[row];
}

template <typename T>
inline T Vector<T>::operator()(uint row) const {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (row >= rows_) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  return data_[row];
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  data_ = std::make_unique<T[]>(other.rows_);
  *data_ = *other.data_;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  if (this != &other) {
    *data_ = *other.data_;
    rows_ = other.rows_;
  }
  return *this;
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  data_ = std::move(other.data_);
  rows_ = std::move(other.rows_);
  other.data_ = nullptr;
  other.rows_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);
    other.data_ = nullptr;
    other.rows_ = 0;
  }
  return *this;
}

}  // namespace libMatrix::inline v1

#endif  // MATRIX_VECTOR_HPP
