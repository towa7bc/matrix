//
// Created by Michael Wittmann on 16/06/2020.
//

#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP

#include <exception>
#include <iostream>
#include <string>
#include <type_traits>

namespace libmatrix::inline v1 {

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
  Vector() : data_(new T[0]){};
  explicit Vector(uint rows);
  Vector(const Vector<T>& /*other*/);
  Vector<T>& operator=(const Vector<T>&);
  Vector(Vector<T>&& /*other*/) noexcept;
  Vector<T>& operator=(Vector<T>&&) noexcept;
  ~Vector() noexcept;
  T& operator()(uint row);
  T operator()(uint row) const;
  inline friend Vector<T> operator+(const Vector<T>& vector1,
                                    const Vector<T>& vector2) {
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  }

  inline friend Vector<T> operator+(Vector<T>&& vector1, Vector<T>&& vector2) {
    Vector<T> result(vector1.rows_);
    for (auto i = 0; i < vector1.rows_; ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  }

  inline friend T operator*(const Vector<T>& vector1,
                            const Vector<T>& vector2) {
    T result;
    for (auto i = 0; i < vector1.rows_; ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  }

  inline friend T operator*(Vector<T>&& vector1, Vector<T>&& vector2) {
    T result;
    for (auto i = 0; i < vector1.rows_; ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  }

  inline friend Vector<T> operator*(T scalarValue, const Vector<T>& vector) {
    Vector<T> result;
    for (auto i = 0; i < vector.rows_; ++i) {
      result(i) = scalarValue * vector(i);
    }
    return result;
  }

  inline friend Vector<T> operator*(const Vector<T>& vector, T scalarValue) {
    Vector<T> result;
    result = scalarValue * vector;
    return result;
  }

  inline friend Vector<T> operator*(Vector<T>&& vector, T scalarValue) {
    Vector<T> result;
    result = scalarValue * vector;
    return result;
  }

  inline friend Vector<T> operator*(T scalarValue, Vector<T>&& vector) {
    Vector<T> result;
    for (auto i = 0; i < vector.rows_; ++i) {
      result(i) = scalarValue * vector(i);
    }
    return result;
  }

  inline friend Vector<T> cross(const Vector<T>& v1, const Vector<T>& v2) {
    std::cout << "cross copy";
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

  inline friend Vector<T> cross(Vector<T>&& v1, Vector<T>&& v2) {
    std::cout << "cross move";
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

  int size() const { return rows_; }

 private:
  uint rows_{0};
  mutable T* data_;
};

template <typename T>
Vector<T>::Vector(uint rows) : rows_(rows) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (rows == 0) {
    throw BadIndexException("Vector constructor has 0 size");
  }
  data_ = new T[rows];
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
Vector<T>::~Vector() noexcept {
  delete[] data_;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) {
  // std::cout << "copy constructor";
  data_ = new T[other.rows_];
  *data_ = *other.data_;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  // std::cout << "copy assignment";
  *data_ = *other.data_;
  rows_ = other.rows_;
  return *this;
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept {
  // std::cout << "move constructor";
  data_ = std::move(other.data_);
  rows_ = std::move(other.rows_);
  other.data_ = nullptr;
  other.rows_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
  std::cout << "move assignment";
  if (this != &other) {
    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);
    other.data_ = nullptr;
    other.rows_ = 0;
  }
  return *this;
}

}  // namespace libmatrix::inline v1

#endif  // MATRIX_VECTOR_HPP
