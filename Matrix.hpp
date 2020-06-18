#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <exception>
#include <iostream>
#include <memory>
#include <type_traits>

#include "Vector.hpp"

namespace libMatrix::inline v1 {

using uint = unsigned int;

template <typename T>
class Matrix {
 public:
  Matrix() = delete;
  Matrix(const Matrix<T>& /*other*/) = delete;
  Matrix<T>& operator=(const Matrix<T>& /*other*/) = delete;
  Matrix(Matrix<T>&& /*other*/) noexcept;
  Matrix<T>& operator=(Matrix<T>&& /*other*/) noexcept;
  Matrix(uint rows, uint cols);
  ~Matrix() noexcept = default;

  [[nodiscard]] bool exists(uint row, uint col) const;
  T& operator()(uint row, uint col);
  T operator()(uint row, uint col) const;

  /// multiply two matrices
  inline friend Matrix<T> operator*(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    if (matrix1.cols_ != matrix2.rows_) {
      throw BadDimensionException(
          "matrix1.cols has to be the same as matrix2.rows");
    }
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

  /// multiply two matrices movable
  inline friend Matrix<T> operator*(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    if (matrix1.cols_ != matrix2.rows_) {
      throw BadDimensionException(
          "matrix1.cols has to be the same as matrix2.rows");
    }
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

  /// multiply a matrix and a vector
  inline friend Vector<T> operator*(const Matrix<T>& matrix,
                                    const Vector<T>& vector) {
    if (matrix.cols_ != vector.rows_) {
      throw BadDimensionException(
          "matrix.cols has to be the same as the vector dimension.");
    }
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i) += matrix(i, j) * vector(j);
      }
    }
    return result;
  }
  /// multiply a transposed vector and a matrix
  inline friend Vector<T> operator*(const Vector<T>& vector,
                                    const Matrix<T>& matrix) {
    if (matrix.rows_ != vector.rows_) {
      throw BadDimensionException(
          "matrix.rows has to be the same as the vector dimension.");
    }
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i) += vector(i) * matrix(i, j);
      }
    }
    return result;
  }

  /// multiply a matrix and a vector movable
  inline friend Vector<T> operator*(Matrix<T>&& matrix, Vector<T>&& vector) {
    if (matrix.cols_ != vector.rows_) {
      throw BadDimensionException(
          "matrix.cols has to be the same as the vector dimension.");
    }
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i) += matrix(i, j) * vector(j);
      }
    }
    return result;
  }
  /// multiply a transposed vector and a matrix movable
  inline friend Vector<T> operator*(Vector<T>&& vector, Matrix<T>&& matrix) {
    if (matrix.rows_ != vector.rows_) {
      throw BadDimensionException(
          "matrix.rows has to be the same as the vector dimension.");
    }
    Vector<T> result(vector.rows_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i) += vector(i) * matrix(i, j);
      }
    }
    return result;
  }

  /// multiply a scalar with a matrix
  inline friend Matrix<T> operator*(T scalarValue, const Matrix<T>& matrix) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i, j) = scalarValue * matrix(i, j);
      }
    }
    return result;
  }

  /// multiply a matrix with a scalar
  inline friend Matrix<T> operator*(const Matrix<T>& matrix, T scalarValue) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    result = scalarValue * matrix;
    return result;
  }

  /// multiply a scalar with a matrix movable
  inline friend Matrix<T> operator*(T scalarValue, Matrix<T>&& matrix) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    for (auto i = 0; i < matrix.rows_; ++i) {
      for (auto j = 0; j < matrix.cols_; ++j) {
        result(i, j) = scalarValue * matrix(i, j);
      }
    }
    return result;
  }

  /// multiply a matrix with a scalar movable
  inline friend Matrix<T> operator*(Matrix<T>&& matrix, T scalarValue) {
    Matrix<T> result(matrix.rows_, matrix.cols_);
    result = scalarValue * matrix;
    return result;
  }

  /// add two matrices
  inline friend Matrix<T> operator+(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    if (matrix1.cols_ != matrix2.cols_ || matrix1.rows_ != matrix2.rows_) {
      throw BadDimensionException("The matrix dimensions have to be the same.");
    }
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

  /// add two matrices movable
  inline friend Matrix<T> operator+(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    if (matrix1.cols_ != matrix2.cols_ || matrix1.rows_ != matrix2.rows_) {
      throw BadDimensionException("The matrix dimensions have to be the same.");
    }
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

  /// subtract two matrices
  inline friend Matrix<T> operator-(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    if (matrix1.cols_ != matrix2.cols_ || matrix1.rows_ != matrix2.rows_) {
      throw BadDimensionException("The matrix dimensions have to be the same.");
    }
    Matrix<T> result(matrix1.rows_, matrix1.cols_);
    for (auto i = 0; i < matrix1.rows_; ++i) {
      for (auto j = 0; j < matrix1.cols_; ++j) {
        for (auto k = 0; k < matrix1.cols_; ++k) {
          result(i, j) = matrix1(i, k) - matrix2(k, j);
        }
      }
    }
    return result;
  }

  /// subtract two matrices movable
  inline friend Matrix<T> operator-(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    if (matrix1.cols_ != matrix2.cols_ || matrix1.rows_ != matrix2.rows_) {
      throw BadDimensionException("The matrix dimensions have to be the same.");
    }
    Matrix<T> result(matrix1.rows_, matrix1.cols_);
    for (auto i = 0; i < matrix1.rows_; ++i) {
      for (auto j = 0; j < matrix1.cols_; ++j) {
        for (auto k = 0; k < matrix1.cols_; ++k) {
          result(i, j) = matrix1(i, k) - matrix2(k, j);
        }
      }
    }
    return result;
  }

  /// print a complete matrix
  inline friend std::ostream& operator<<(std::ostream& out,
                                         const Matrix<T>& m) {
    for (auto row{0}; row < m.rows_; ++row) {
      out << '|' << ' ';
      for (auto col{0}; col < m.cols_; ++col) {
        out << m(row, col) << ' ';
      }
      out << '|' << '\n';
    }
    return out;
  }

  /// print a complete matrix movable
  inline friend std::ostream& operator<<(std::ostream& out, Matrix<T>&& m) {
    for (auto row{0}; row < m.rows_; ++row) {
      out << '|' << ' ';
      for (auto col{0}; col < m.cols_; ++col) {
        out << m(row, col) << ' ';
      }
      out << '|' << '\n';
    }
    return out;
  }

  [[nodiscard]] int size() const { return rows_ * cols_; }
  [[nodiscard]] int rows() const { return rows_; }
  [[nodiscard]] int cols() const { return cols_; }

 private:
  uint rows_{0}, cols_{0};
  std::unique_ptr<T[]> data_;
};

template <typename T>
Matrix<T>::Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  if (rows == 0 || cols == 0) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  data_ = std::make_unique<T[]>(rows * cols);
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
Matrix<T>::Matrix(Matrix<T>&& other) noexcept
    : data_(std::move(other.data_)),
      rows_(std::move(other.rows_)),
      cols_(std::move(other.cols_)) {
  // std::cout << "move constructor";
  static_assert(std::is_arithmetic_v<T>, "Arithmetic required.");
  other.data_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
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

}  // namespace libMatrix::inline v1

#endif  // MATRIX_MATRIX_HPP
