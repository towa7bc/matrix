#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <exception>
#include <iostream>
#include <type_traits>
#include <vector>

#include "Interfaces.hpp"
#include "MatrixHelperFunctions.hpp"
#include "Vector.hpp"

namespace libMatrix::inline v1 {

template <Arithmetic T>
class Matrix : private IEqualityComparable<Matrix, T>,
               private IArithmeticOperations<Matrix, T>,
               private IMixedArithmeticOperations<Matrix, Vector, T>,
               private IGenericOperations<Matrix, T> {
 private:
  std::vector<T> data_;
  size_t rows_{0UL}, cols_{0UL};

 public:
  explicit Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
    if (rows == 0 || cols == 0) {
      throw BadIndexException("Matrix constructor has 0 size");
    }
    data_.resize(rows * cols);
  }

  Matrix(std::initializer_list<T> init, size_t rows, size_t cols)
      : data_(std::move(init)), rows_(rows), cols_(cols) {
    if (init.size() == 0) {
      throw BadIndexException("Matrix constructor has 0 size");
    }
  }

  Arithmetic auto& operator()(size_t row, size_t col) {
    if (row >= rows_ || col >= cols_) {
      throw BadIndexException("Matrix index invalid");
    }
    return data_[cols_ * row + col];
  }

  Arithmetic auto operator()(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
      throw BadIndexException("Matrix index invalid");
    }
    return data_[cols_ * row + col];
  }

  [[nodiscard]] bool exists(size_t row, size_t col) const {
    return (row < rows_ && col < cols_);
  }

  [[nodiscard]] size_t size() const { return rows_ * cols_; }
  [[nodiscard]] size_t rows() const { return rows_; }
  [[nodiscard]] size_t cols() const { return cols_; }

  void swap(Matrix& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
    swap(cols_, rhs.cols_);
  }
};

}  // namespace libMatrix::inline v1

#endif  // MATRIX_MATRIX_HPP
