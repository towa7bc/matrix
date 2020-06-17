//
// Created by Michael Wittmann on 16/06/2020.
//

#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <exception>
#include <iostream>
#include <type_traits>

#include "Vector.hpp"

namespace libmatrix::inline v1 {

using uint = unsigned int;

template <typename T>
class Matrix {
 public:
  Matrix() : data_(new T[0]){};
  Matrix(const Matrix<T>& /*other*/);
  Matrix<T>& operator=(const Matrix<T>&);
  Matrix(Matrix<T>&& /*other*/) noexcept;
  Matrix<T>& operator=(Matrix<T>&&) noexcept;
  Matrix(uint rows, uint cols);
  ~Matrix() noexcept;

  bool exists(uint row, uint col) const;
  T& operator()(uint row, uint col);
  T operator()(uint row, uint col) const;
  inline friend Matrix<T> operator*(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    Matrix<T> result(matrix1.rows_, matrix1.cols_);
    for (auto i = 0; i < matrix1.rows_; ++i) {
      for (auto j = 0; j < matrix1.cols_; ++j) {
        for (auto k = 0; k < matrix1.cols_; ++k) {
          result(i, j) += matrix1(i, k) * matrix2(k, j);
        }
      }
    }
    return result;
  }

  inline friend Vector<T> operator*(const Matrix<T>& matrix,
                                    const Vector<T>& vector) {
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i) += matrix(i, j) * vector(j);
      }
    }
    return result;
  }

  inline friend Matrix<T> operator*(T scalarValue, const Matrix<T>& matrix) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i, j) = scalarValue * matrix(i, j);
      }
    }
    return result;
  }

  inline friend Matrix<T> operator*(const Matrix<T>& matrix, T scalarValue) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    result = scalarValue * matrix;
    return result;
  }

  inline friend Matrix<T> operator+(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    Matrix<T> result(matrix1.rows_, matrix1.cols_);
    for (auto i = 0; i < matrix1.rows_; ++i) {
      for (auto j = 0; j < matrix1.cols_; ++j) {
        for (auto k = 0; k < matrix1.cols_; ++k) {
          result(i, j) = matrix1(i, k) + matrix2(k, j);
        }
      }
    }
    return result;
  }

  int size() const { return rows_ * cols_; }
  int rows() const { return rows_; }
  int cols() const { return cols_; }

 private:
  uint rows_{0}, cols_{0};
  mutable T* data_;
};

template <typename T>
Matrix<T>::Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (rows == 0 || cols == 0) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  data_ = new T[rows * cols];
}

template <typename T>
inline T& Matrix<T>::operator()(uint row, uint col) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (row >= rows_ || col >= cols_) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  return data_[cols_ * row + col];
}

template <typename T>
inline T Matrix<T>::operator()(uint row, uint col) const {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (row >= rows_ || col >= cols_) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  return data_[cols_ * row + col];
}

template <typename T>
bool Matrix<T>::exists(uint row, uint col) const {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  return (row < rows_ && col < cols_);
}

template <typename T>
Matrix<T>::~Matrix() noexcept {
  // std::cout << "destructor";
  delete[] data_;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) {
  // std::cout << "copy constructor";
  data_ = new T[other.rows_ * other.cols_];
  *data_ = *other.data_;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  // std::cout << "copy assignment";
  *data_ = *other.data_;
  cols_ = other.cols_;
  rows_ = other.rows_;
  return *this;
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept
    : data_(std::move(other.data_)),
      rows_(std::move(other.rows_)),
      cols_(std::move(other.cols_)) {
  // std::cout << "move constructor";
  other.data_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
  std::cout << "move assignment";
  if (this != &other) {
    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);
    cols_ = std::move(other.cols_);
    other.data_ = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
  }
  return *this;
}

}  // namespace libmatrix::inline v1

#endif  // MATRIX_MATRIX_HPP