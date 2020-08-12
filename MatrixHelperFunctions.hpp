#ifndef MATRIX_MATRIXHELPERFUNCTIONS_HPP
#define MATRIX_MATRIXHELPERFUNCTIONS_HPP

#include <type_traits>
#include <iostream>
#include <concepts>
#include <exception>
#include <future>

namespace libMatrix::inline v1 {

template <Arithmetic T>
std::ostream& print_matrix(std::ostream& out, const Matrix<T>& m) {
  for (size_t row{0UL}; row < m.rows(); ++row) {
    out << '|' << ' ';
    for (size_t col{0UL}; col < m.cols(); ++col) {
      out << m(row, col) << ' ';
    }
    out << '|' << '\n';
  }
  return out;
}

template <Arithmetic T>
Matrix<T> subtract_matrices(const Matrix<T>& matrix1,
                            const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix1.cols());
    for (size_t i{0UL}; i < matrix1.rows(); ++i) {
      for (size_t j{0UL}; j < matrix1.cols(); ++j) {
        result(i, j) = matrix1(i, j) - matrix2(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Matrix<T> add_matrices(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix1.cols());
    for (size_t i{0UL}; i < matrix1.rows(); ++i) {
      for (size_t j{0UL}; j < matrix1.cols(); ++j) {
        result(i, j) = matrix1(i, j) + matrix2(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Matrix<T> multiply_scalar_matrix(Arithmetic auto scalarValue,
                                 const Matrix<T>& matrix) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  for (size_t i{0UL}; i < matrix.rows(); ++i) {
    for (size_t j{0UL}; j < matrix.cols(); ++j) {
      result(i, j) = scalarValue * matrix(i, j);
    }
  }
  return result;
}

template <Arithmetic T>
Matrix<T> multiply_matrix_scalar(const Matrix<T>& matrix,
                                 Arithmetic auto scalarValue) {
  Matrix<T> result(matrix.rows(), matrix.cols());
  result = scalarValue * matrix;
  return result;
}

template <Arithmetic T>
Vector<T> multiply_matrix_vector(const Matrix<T>& matrix,
                                 const Vector<T>& vector) {
  if (matrix.cols() != vector.size()) {
    throw BadDimensionException(
        "matrix.cols has to be the same as the vector dimension.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector.size());
    for (size_t i{0UL}; i < matrix.rows(); ++i) {
      for (size_t j{0UL}; j < matrix.cols(); ++j) {
        result(i) += matrix(i, j) * vector(j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Vector<T> multiply_matrix_transposed_vector(const Vector<T>& vector,
                                            const Matrix<T>& matrix) {
  if (matrix.rows() != vector.size()) {
    throw BadDimensionException(
        "matrix.rows has to be the same as the vector dimension.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector.size());
    for (size_t i{0UL}; i < matrix.rows(); ++i) {
      for (size_t j{0UL}; j < matrix.cols(); ++j) {
        result(i) += vector(i) * matrix(i, j);
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Matrix<T> multiply_matrices(const Matrix<T>& matrix1,
                            const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.rows()) {
    throw BadDimensionException(
        "matrix1.cols has to be the same as matrix2.rows");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Matrix<T> result(matrix1.rows(), matrix2.cols());
    for (size_t i{0UL}; i < matrix1.rows(); ++i) {
      for (size_t j{0UL}; j < matrix2.cols(); ++j) {
        for (size_t k{0UL}; k < matrix1.cols(); ++k) {
          result(i, j) += matrix1(i, k) * matrix2(k, j);
        }
      }
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
bool equal_matrices(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
  if (matrix1.cols() != matrix2.cols() || matrix1.rows() != matrix2.rows()) {
    throw BadDimensionException("The matrix dimensions have to be the same.");
  }
  bool returnValue =
      matrix1.cols() == matrix2.cols() || matrix1.rows() == matrix2.rows();
  for (size_t i{0UL}; i < matrix1.rows(); ++i) {
    for (size_t j{0UL}; j < matrix1.cols(); ++j) {
      returnValue &= matrix1(i, j) == matrix2(i, j);
    }
  }
  return returnValue;
}

}

#endif  // MATRIX_MATRIXHELPERFUNCTIONS_HPP
