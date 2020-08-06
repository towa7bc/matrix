#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP

#include <concepts>
#include <exception>
#include <future>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Interfaces.hpp"
#include "VectorHelperFunctions.hpp"

namespace libMatrix::inline v1 {

template <ArithmeticNoBool T>
class Vector : private IEqualityComparable<Vector<T>, T>,
               private IArithmeticOperations<Vector<T>, T>,
               private IGenericOperations<Vector<T>, T> {
 private:
  size_t rows_{0};
  std::vector<T> data_;

 public:
  explicit Vector(size_t rows) : rows_(rows) {
    if (rows == 0) {
      throw BadIndexException("Vector constructor has 0 size");
    }
    data_.resize(rows);
  }

  Vector(std::initializer_list<T> init)
      : data_(std::move(init)), rows_(init.size()) {
    if (init.size() == 0) {
      throw BadIndexException("Vector constructor has 0 size");
    }
  }

  T& operator()(size_t row) {
    if (row >= rows_) {
      throw BadIndexException("Vector index invalid");
    }
    return data_[row];
  }

  T operator()(size_t row) const {
    if (row >= rows_) {
      throw BadIndexException("Vector index invalid");
    }
    return data_[row];
  }

  [[nodiscard]] int size() const { return rows_; }

  void swap(Vector& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
  }
};

}  // namespace libMatrix::inline v1

#endif  // MATRIX_VECTOR_HPP
