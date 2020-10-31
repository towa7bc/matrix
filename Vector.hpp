#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP

#include <fmt/format.h>

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
  [[nodiscard]] auto data() const { return data_; }

  void swap(Vector& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
  }
};

}  // namespace libMatrix::inline v1

template <typename T>
struct fmt::formatter<libMatrix::Vector<T>> {
  constexpr auto parse(format_parse_context& ctx) {
    value_format_ = "{:";
    for (const auto* it = ctx.begin(); it != ctx.end(); ++it) {
      char c = *it;
      value_format_ += c;
      if (c == '}') {
        return it;
      }
    }
    return ctx.end();
  }

  template <typename FormatContext>
  auto format(libMatrix::Vector<T> const& v, FormatContext& ctx) {
    auto&& out = ctx.out();
    format_to(out, "[");
    if (v.size() > 0) {
      format_to(out, value_format_, v(0));
    }
    for (std::size_t i{1}; i < v.size(); ++i) {
      format_to(out, ", " + value_format_, v(i));
    }
    return format_to(out, "]");
  }

  std::string value_format_;
};

#endif  // MATRIX_VECTOR_HPP
