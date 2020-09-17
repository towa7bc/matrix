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

using size_t = std::size_t;

template <Arithmetic T>
class Vector : private IEqualityComparable<Vector, T>,
               private IArithmeticOperations<Vector, T>,
               private IGenericOperations<Vector, T> {
 private:
  std::vector<T> data_;
  size_t rows_{0UL};

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

  Arithmetic auto& operator()(size_t row) {
    if (row >= rows_) {
      throw BadIndexException("Vector index invalid");
    }
    return data_[row];
  }

  Arithmetic auto operator()(size_t row) const {
    if (row >= rows_) {
      throw BadIndexException("Vector index invalid");
    }
    return data_[row];
  }

  [[nodiscard]] size_t size() const { return rows_; }

  void swap(Vector& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
  }
};

}  // namespace libMatrix::inline v1

#endif  // MATRIX_VECTOR_HPP
