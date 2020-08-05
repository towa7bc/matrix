#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <exception>
#include <future>
#include <iostream>
#include <type_traits>
#include <vector>

#include "Vector.hpp"

namespace libMatrix::inline v1 {

template <typename Mat>
class IMatrixEqualityComparable {
 private:
  /// matrix equality
  friend bool operator==(Mat matrix1, Mat matrix2) {
    return equal_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// matrix inequality
  friend bool operator!=(Mat matrix1, Mat matrix2) {
    return !(std::move(matrix1) == std::move(matrix2));
  }
};

template <typename Mat, typename Vec, typename Scalar>
class IMatrixArithmeticOperations {
 private:
  /// multiply two matrices
  friend Mat operator*(Mat matrix1, Mat matrix2) {
    return multiply_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// multiply a matrix and a vector
  friend Vec operator*(Mat matrix, Vec vector) {
    return multiply_matrix_vector(std::move(matrix), std::move(vector));
  }
  /// multiply a transposed vector and a matrix
  friend Vec operator*(Vec vector, Mat matrix) {
    return multiply_matrix_transposed_vector(std::move(vector),
                                             std::move(matrix));
  }

  /// multiply a scalar with a matrix
  friend Mat operator*(Scalar scalarValue, Mat matrix) {
    return multiply_scalar_matrix(scalarValue, std::move(matrix));
  }

  /// multiply a matrix with a scalar
  friend Mat operator*(Mat matrix, Scalar scalarValue) {
    return multiply_matrix_scalar(std::move(matrix), scalarValue);
  }

  /// add two matrices
  friend Mat operator+(Mat matrix1, Mat matrix2) {
    return add_matrices(std::move(matrix1), std::move(matrix2));
  }

  /// subtract two matrices
  friend Mat operator-(Mat matrix1, Mat matrix2) {
    return subtract_matrices(std::move(matrix1), std::move(matrix2));
  }
};

template <typename Mat>
class IMatrixGenericOperations {
 private:
  /// swap matrices
  friend void swap(Mat& m1, Mat& m2) noexcept { m1.swap(m2); }

  /// print a complete matrix
  friend std::ostream& operator<<(std::ostream& out, Mat m) {
    return print_matrix(out, std::move(m));
  }
};

template <ArithmeticNoBool T>
class Matrix : private IMatrixEqualityComparable<Matrix<T>>,
               private IMatrixArithmeticOperations<Matrix<T>, Vector<T>, T>,
               private IMatrixGenericOperations<Matrix<T>> {
 private:
  uint rows_{0}, cols_{0};
  std::vector<T> data_;

 public:
  explicit Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
    if (rows == 0 || cols == 0) {
      throw BadIndexException("Matrix constructor has 0 size");
    }
    data_.resize(rows * cols);
  }

  T& operator()(uint row, uint col) {
    if (row >= rows_ || col >= cols_) {
      throw BadIndexException("Matrix constructor has 0 size");
    }
    return data_[cols_ * row + col];
  }

  T operator()(uint row, uint col) const {
    if (row >= rows_ || col >= cols_) {
      throw BadIndexException("Matrix constructor has 0 size");
    }
    return data_[cols_ * row + col];
  }

  [[nodiscard]] bool exists(uint row, uint col) const {
    return (row < rows_ && col < cols_);
  }

  [[nodiscard]] int size() const { return rows_ * cols_; }
  [[nodiscard]] int rows() const { return rows_; }
  [[nodiscard]] int cols() const { return cols_; }

  void swap(Matrix& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
    swap(cols_, rhs.cols_);
  }
};

/// #region helper functions

template <ArithmeticNoBool T>
std::ostream& print_matrix(std::ostream& out, const Matrix<T>& m) {
  for (auto row{0}; row < m.rows(); ++row) {
    out << '|' << ' ';
    for (auto col{0}; col < m.cols(); ++col) {
      out << m(row, col) << ' ';
    }
    out << '|' << '\n';
  }
  return out;
}

template <ArithmeticNoBool T>
Matrix<T> subtract_matrices(const Matrix<T>& matrix1,
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

template <ArithmeticNoBool T>
Matrix<T> add_matrices(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
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

template <ArithmeticNoBool T>
Matrix<T> multiply_scalar_matrix(T scalarValue, const Matrix<T>& matrix) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  for (auto i = 0; i < matrix.rows(); ++i) {
    for (auto j = 0; j < matrix.cols(); ++j) {
      result(i, j) = scalarValue * matrix(i, j);
    }
  }
  return result;
}

template <ArithmeticNoBool T>
Matrix<T> multiply_matrix_scalar(const Matrix<T>& matrix, T scalarValue) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  result = scalarValue * matrix;
  return result;
}

template <ArithmeticNoBool T>
Vector<T> multiply_matrix_vector(const Matrix<T>& matrix,
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

template <ArithmeticNoBool T>
Vector<T> multiply_matrix_transposed_vector(const Vector<T>& vector,
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

template <ArithmeticNoBool T>
Matrix<T> multiply_matrices(const Matrix<T>& matrix1,
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

template <ArithmeticNoBool T>
bool equal_matrices(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
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
