#ifndef MATRIX_INTERFACES_HPP
#define MATRIX_INTERFACES_HPP

#include <type_traits>
#include <iostream>
#include <concepts>
#include <exception>

namespace libMatrix::inline v1 {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T> && !std::is_same_v<bool, T>;

template <Arithmetic T>
class Vector;

template <Arithmetic T>
class Matrix;

template <typename VecOrMat, Arithmetic T>
class IEqualityComparable {
 private:
  /// vector/matrix equality
  friend bool operator==(VecOrMat item1, VecOrMat item2) {
    if constexpr (std::is_same_v<Vector<T>, VecOrMat>) {
      return equal_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<T>, VecOrMat>) {
      return equal_matrices(std::move(item1), std::move(item2));
    }
  }

  /// vector/matrix inequality
  friend bool operator!=(VecOrMat item1, VecOrMat item2) {
    return !(std::move(item1) == std::move(item2));
  }
};

template <typename VecOrMat, Arithmetic Scalar>
class IArithmeticOperations {
 private:
  /// vector/matrix addition
  friend VecOrMat operator+(VecOrMat item1, VecOrMat item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return add_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat>) {
      return add_matrices(std::move(item1), std::move(item2));
    }
  }

  /// vector/matrix subtraction
  friend VecOrMat operator-(VecOrMat item1, VecOrMat item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return subtract_vectors(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat>) {
      return subtract_matrices(std::move(item1), std::move(item2));
    }
  }

  /// scalar multiplication of vectors/matrix multiplication
  friend auto operator*(VecOrMat item1, VecOrMat item2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return scalar_product(std::move(item1), std::move(item2));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat>) {
      return multiply_matrices(std::move(item1), std::move(item2));
    }
  }

  /// multiply a scalar value with a vector/matrix
  friend VecOrMat operator*(Scalar scalarValue, VecOrMat item) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return multiply_scalar_vector(scalarValue, std::move(item));
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat>) {
      return multiply_scalar_matrix(scalarValue, std::move(item));
    }
  }

  /// multiply a vector/matrix with a scalar value
  friend VecOrMat operator*(VecOrMat item, Scalar scalarValue) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return multiply_vector_scalar(std::move(item), scalarValue);
    } else if constexpr (std::is_same_v<Matrix<Scalar>, VecOrMat>) {
      return multiply_matrix_scalar(std::move(item), scalarValue);
    }
  }

  /// calculate the cross product of two 3d vectors
  friend VecOrMat cross(VecOrMat v1, VecOrMat v2) {
    if constexpr (std::is_same_v<Vector<Scalar>, VecOrMat>) {
      return calc_cross(std::move(v1), std::move(v2));
    }
  }
};

template <typename Mat, typename Vec>
class IMixedArithmeticOperations {
 private:
  /// multiply a matrix and a vector
  friend Vec operator*(Mat matrix, Vec vector) {
    return multiply_matrix_vector(std::move(matrix), std::move(vector));
  }

  /// multiply a transposed vector and a matrix
  friend Vec operator*(Vec vector, Mat matrix) {
    return multiply_matrix_transposed_vector(std::move(vector),
                                             std::move(matrix));
  }
};

template <typename VecOrMat, Arithmetic T>
class IGenericOperations {
 private:
  /// swap vectors/matrices
  friend void swap(VecOrMat& v1, VecOrMat& v2) noexcept { v1.swap(v2); }

  /// print a complete vector/matrix
  friend std::ostream& operator<<(std::ostream& out, VecOrMat v) {
    if constexpr (std::is_same_v<Vector<T>, VecOrMat>) {
      return print_vector(out, std::move(v));
    } else if constexpr (std::is_same_v<Matrix<T>, VecOrMat>) {
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

}

#endif  // MATRIX_INTERFACES_HPP
