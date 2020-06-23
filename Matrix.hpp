#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <exception>
#include <future>
#include <iostream>
#include <type_traits>
#include <vector>

#include "Vector.hpp"

namespace libMatrix::inline v1 {

using uint = unsigned int;

template <Arithmetic T>
class Matrix {
 public:
  Matrix() = delete;
  explicit Matrix(uint /*rows*/, uint /*cols*/);
  Matrix(const Matrix<T>& /*other*/);
  Matrix<T>& operator=(const Matrix<T>& /*other*/);
  Matrix(Matrix<T>&& /*other*/) noexcept;
  Matrix<T>& operator=(Matrix<T>&& /*other*/) noexcept;
  ~Matrix() noexcept = default;

  T& operator()(uint /*row*/, uint /*col*/);
  T operator()(uint /*row*/, uint /*col*/) const;

  [[nodiscard]] bool exists(uint /*row*/, uint /*col*/) const;
  [[nodiscard]] int size() const { return rows_ * cols_; }
  [[nodiscard]] int rows() const { return rows_; }
  [[nodiscard]] int cols() const { return cols_; }

  /// matrix equality
  inline friend bool operator==(const Matrix<T>& matrix1,
                                const Matrix<T>& matrix2) {
    return equal_matrices(matrix1, matrix2);
  }

  /// matrix inequality
  inline friend bool operator!=(const Matrix<T>& matrix1,
                                const Matrix<T>& matrix2) {
    return !(matrix1 == matrix2);
  }

  /// multiply two matrices
  inline friend Matrix<T> operator*(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    return multiply_matrices(matrix1, matrix2);
  }

  /// multiply two matrices movable
  inline friend Matrix<T> operator*(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    return multiply_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// multiply a matrix and a vector
  inline friend Vector<T> operator*(const Matrix<T>& matrix,
                                    const Vector<T>& vector) {
    return multiply_matrix_vector(matrix, vector);
  }
  /// multiply a transposed vector and a matrix
  inline friend Vector<T> operator*(const Vector<T>& vector,
                                    const Matrix<T>& matrix) {
    return multiply_matrix_transposed_vector(vector, matrix);
  }

  /// multiply a matrix and a vector movable
  inline friend Vector<T> operator*(Matrix<T>&& matrix, Vector<T>&& vector) {
    return multiply_matrix_vector(std::move(matrix), std::move(vector));
  }

  /// multiply a transposed vector and a matrix movable
  inline friend Vector<T> operator*(Vector<T>&& vector, Matrix<T>&& matrix) {
    return multiply_matrix_transposed_vector(std::move(vector),
                                             std::move(matrix));
  }

  /// multiply a scalar with a matrix
  inline friend Matrix<T> operator*(T scalarValue, const Matrix<T>& matrix) {
    return multiply_scalar_matrix(scalarValue, matrix);
  }

  /// multiply a matrix with a scalar
  inline friend Matrix<T> operator*(const Matrix<T>& matrix, T scalarValue) {
    return multiply_matrix_scalar(matrix, scalarValue);
  }

  /// multiply a scalar with a matrix movable
  inline friend Matrix<T> operator*(T scalarValue, Matrix<T>&& matrix) {
    return multiply_scalar_matrix(scalarValue, std::move(matrix));
  }

  /// multiply a matrix with a scalar movable
  inline friend Matrix<T> operator*(Matrix<T>&& matrix, T scalarValue) {
    return multiply_matrix_scalar(std::move(matrix), scalarValue);
  }

  /// add two matrices
  inline friend Matrix<T> operator+(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    return add_matrices(matrix1, matrix2);
  }

  /// add two matrices movable
  inline friend Matrix<T> operator+(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    return add_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// subtract two matrices
  inline friend Matrix<T> operator-(const Matrix<T>& matrix1,
                                    const Matrix<T>& matrix2) {
    return subtract_matrices(matrix1, matrix2);
  }

  /// subtract two matrices movable
  inline friend Matrix<T> operator-(Matrix<T>&& matrix1, Matrix<T>&& matrix2) {
    return subtract_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// print a complete matrix
  inline friend std::ostream& operator<<(std::ostream& out,
                                         const Matrix<T>& m) {
    return print_matrix(out, m);
  }

  /// print a complete matrix movable
  inline friend std::ostream& operator<<(std::ostream& out, Matrix<T>&& m) {
    return print_matrix(out, std::move(m));
  }

 private:
  uint rows_{0}, cols_{0};
  std::vector<T> data_;
};

/// #region class implementation

template <Arithmetic T>
Matrix<T>::Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
  if (rows == 0 || cols == 0) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  data_.resize(rows * cols);
}

template <Arithmetic T>
inline T& Matrix<T>::operator()(uint row, uint col) {
  if (row >= rows_ || col >= cols_) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  return data_[cols_ * row + col];
}

template <Arithmetic T>
inline T Matrix<T>::operator()(uint row, uint col) const {
  if (row >= rows_ || col >= cols_) {
    throw BadIndexException("Matrix constructor has 0 size");
  }
  return data_[cols_ * row + col];
}

template <Arithmetic T>
bool Matrix<T>::exists(uint row, uint col) const {
  return (row < rows_ && col < cols_);
}

template <Arithmetic T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : cols_(other.cols_), rows_(other.rows_) {
  data_.resize(other.rows_ * other.cols_);
  data_ = other.data_;
}

template <Arithmetic T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (this != &other) {
    data_.resize(other.rows_ * other.cols_);
    data_ = other.data_;
    cols_ = other.cols_;
    rows_ = other.rows_;
  }
  return *this;
}

template <Arithmetic T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept
    : data_(std::move(other.data_)),
      rows_(std::move(other.rows_)),
      cols_(std::move(other.cols_)) {
  other.data_.clear();
  other.rows_ = 0;
  other.cols_ = 0;
}

template <Arithmetic T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);
    cols_ = std::move(other.cols_);
    other.data_.clear();
    other.rows_ = 0;
    other.cols_ = 0;
  }
  return *this;
}

/// #endregion class implementation

/// #region helper functions

template <Arithmetic T>
inline std::ostream& print_matrix(std::ostream& out, const Matrix<T>& m) {
  for (auto row{0}; row < m.rows(); ++row) {
    out << '|' << ' ';
    for (auto col{0}; col < m.cols(); ++col) {
      out << m(row, col) << ' ';
    }
    out << '|' << '\n';
  }
  return out;
}

template <Arithmetic T>
inline Matrix<T> subtract_matrices(const Matrix<T>& matrix1,
                                   const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix1.cols());
    for (auto i = 0; i < matrix1.rows(); ++i) {
      for (auto j = 0; j < matrix1.cols(); ++j) {
        result(i, j) = matrix1(i, j) - matrix2(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
inline Matrix<T> add_matrices(const Matrix<T>& matrix1,
                              const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix1.cols());
    for (auto i = 0; i < matrix1.rows(); ++i) {
      for (auto j = 0; j < matrix1.cols(); ++j) {
        result(i, j) = matrix1(i, j) + matrix2(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
inline Matrix<T> multiply_scalar_matrix(T scalarValue,
                                        const Matrix<T>& matrix) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  for (auto i = 0; i < matrix.rows(); ++i) {
    for (auto j = 0; j < matrix.cols(); ++j) {
      result(i, j) = scalarValue * matrix(i, j);
    }
  }
  return result;
}

template <Arithmetic T>
inline Matrix<T> multiply_matrix_scalar(const Matrix<T>& matrix,
                                        T scalarValue) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  result = scalarValue * matrix;
  return result;
}

template <Arithmetic T>
inline Vector<T> multiply_matrix_vector(const Matrix<T>& matrix,
                                        const Vector<T>& vector) {
  if (matrix.cols() != vector.size()) {
    throw BadDimensionException(
        "matrix.cols has to be the same as the vector dimension.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector.size());
    for (auto i = 0; i < matrix.rows(); ++i) {
      for (auto j = 0; j < matrix.cols(); ++j) {
        result(i) += matrix(i, j) * vector(j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
inline Vector<T> multiply_matrix_transposed_vector(const Vector<T>& vector,
                                                   const Matrix<T>& matrix) {
  if (matrix.rows() != vector.size()) {
    throw BadDimensionException(
        "matrix.rows has to be the same as the vector dimension.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector.size());
    for (auto i = 0; i < matrix.rows(); ++i) {
      for (auto j = 0; j < matrix.cols(); ++j) {
        result(i) += vector(i) * matrix(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
inline Matrix<T> multiply_matrices(const Matrix<T>& matrix1,
                                   const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.rows()) {
    throw BadDimensionException(
        "matrix1.cols has to be the same as matrix2.rows");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix2.cols());
    for (auto i = 0; i < matrix1.rows(); ++i) {
      for (auto j = 0; j < matrix2.cols(); ++j) {
        for (auto k = 0; k < matrix1.cols(); ++k) {
          result(i, j) += matrix1(i, k) * matrix2(k, j);
        }
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
inline bool equal_matrices(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  bool returnValue =
      matrix1.cols() == matrix2.cols() || matrix1.rows() == matrix2.rows();
  for (auto i = 0; i < matrix1.rows(); ++i) {
    for (auto j = 0; j < matrix1.cols(); ++j) {
      returnValue &= matrix1(i, j) == matrix2(i, j);
    }
  }
  return returnValue;
}

/// #endregion helper functions

}  // namespace libMatrix::inline v1

#endif  // MATRIX_MATRIX_HPP
