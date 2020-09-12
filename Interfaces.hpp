#ifndef MATRIX_INTERFACES_HPP
#define MATRIX_INTERFACES_HPP

#include <concepts>
#include <exception>
#include <iostream>
#include <type_traits>

namespace libMatrix::inline v1 {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T> && !std::is_same_v<bool, T>;

template <Arithmetic T>
class Vector;

template <Arithmetic T>
class Matrix;

template <template <Arithmetic> typename VecOrMat, Arithmetic T>
class IEqualityComparable {
 private:
  /// vector/matrix equality
  friend bool operator==(VecOrMat<T> item1, VecOrMat<T> item2) {
    if constexpr (std::is_same_v<Vector<T>, VecOrMat<T>>) {
      return equal_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<T>, VecOrMat<T>>) {
      return equal_matrices(std::move(item1), std::move(item2));
    }
  }

  /// vector/matrix inequality
  friend bool operator!=(VecOrMat<T> item1, VecOrMat<T> item2) {
    return !(std::move(item1) == std::move(item2));
  }
};

template <template <Arithmetic> typename VecOrMat, Arithmetic Scalar>
class IArithmeticOperations {
 private:
  /// vector/matrix addition
  friend VecOrMat<Scalar> operator+(VecOrMat<Scalar> item1,
                                    VecOrMat<Scalar> item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return add_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat<Scalar>>) {
      return add_matrices(std::move(item1), std::move(item2));
    }
  }

  /// vector/matrix subtraction
  friend VecOrMat<Scalar> operator-(VecOrMat<Scalar> item1,
                                    VecOrMat<Scalar> item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return subtract_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat<Scalar>>) {
      return subtract_matrices(std::move(item1), std::move(item2));
    }
  }

  /// scalar multiplication of vectors/matrix multiplication
  friend auto operator*(VecOrMat<Scalar> item1, VecOrMat<Scalar> item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return scalar_product(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat<Scalar>>) {
      return multiply_matrices(std::move(item1), std::move(item2));
    }
  }

  /// multiply a scalar value with a vector/matrix
  friend VecOrMat<Scalar> operator*(Scalar scalarValue, VecOrMat<Scalar> item) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return multiply_scalar_vector(scalarValue, std::move(item));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat<Scalar>>) {
      return multiply_scalar_matrix(scalarValue, std::move(item));
    }
  }

  /// multiply a vector/matrix with a scalar value
  friend VecOrMat<Scalar> operator*(VecOrMat<Scalar> item, Scalar scalarValue) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return multiply_vector_scalar(std::move(item), scalarValue);
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat<Scalar>>) {
      return multiply_matrix_scalar(std::move(item), scalarValue);
    }
  }

  /// calculate the cross product of two 3d vectors
  friend VecOrMat<Scalar> cross(VecOrMat<Scalar> v1, VecOrMat<Scalar> v2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat<Scalar>>) {
      return calc_cross(std::move(v1), std::move(v2));
    }
  }
};

template <template <Arithmetic> typename Mat,
          template <Arithmetic> typename Vec, Arithmetic T>
class IMixedArithmeticOperations {
 private:
  /// multiply a matrix and a vector
  friend Vec<T> operator*(Mat<T> matrix, Vec<T> vector) {
    return multiply_matrix_vector(std::move(matrix), std::move(vector));
  }

  /// multiply a transposed vector and a matrix
  friend Vec<T> operator*(Vec<T> vector, Mat<T> matrix) {
    return multiply_matrix_transposed_vector(std::move(vector),
                                             std::move(matrix));
  }
};

template <template <Arithmetic> typename VecOrMat, Arithmetic T>
class IGenericOperations {
 private:
  /// swap vectors/matrices
  friend void swap(VecOrMat<T>& v1, VecOrMat<T>& v2) noexcept { v1.swap(v2); }

  /// print a complete vector/matrix
  friend std::ostream& operator<<(std::ostream& out, VecOrMat<T> v) {
    if constexpr (std::is_same_v<Vector<T>, VecOrMat<T>>) {
      return print_vector(out, std::move(v));
    } else if constexpr (std::is_same_v<Matrix<T>, VecOrMat<T>>) {
      return print_matrix(out, std::move(v));
    }
  }
};

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

}  // namespace libMatrix::inline v1

#endif  // MATRIX_INTERFACES_HPP
