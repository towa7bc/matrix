#ifndef MATRIX_VECTORHELPERFUNCTIONS_HPP
#define MATRIX_VECTORHELPERFUNCTIONS_HPP

#include <concepts>
#include <exception>
#include <future>
#include <iostream>
#include <type_traits>

namespace libMatrix::inline v1 {

template <Arithmetic T>
std::ostream& print_vector(std::ostream& out, const Vector<T>& v) {
  for (size_t index{0UL}; index < v.size(); ++index) {
    out << '|' << ' ' << v(index) << ' ' << '|' << '\n';
  }
  return out;
}

template <Arithmetic T>
Vector<T> calc_cross(const Vector<T>& v1, const Vector<T>& v2) {
  if (v1.size() != 3 || v2.size() != 3) {
    throw BadDimensionException(
        "Vector cross product is only defined in 3 dimensions.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(3);
    result(0) = v1(1) * v2(2) - v1(2) * v2(1);
    result(1) = -(v1(0) * v2(2) - v1(2) * v2(0));
    result(2) = v1(0) * v2(1) - v1(1) * v2(0);
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Vector<T> multiply_scalar_vector(Arithmetic auto scalarValue,
                                 const Vector<T>& vector) {
  Vector<T> result(vector.size());
  for (size_t i{0UL}; i < vector.size(); ++i) {
    result(i) = scalarValue * vector(i);
  }
  return result;
}

template <Arithmetic T>
Vector<T> multiply_vector_scalar(const Vector<T>& vector,
                                 Arithmetic auto scalarValue) {
  Vector<T> result(vector.size());
  result = scalarValue * vector;
  return result;
}

template <Arithmetic T>
Arithmetic auto scalar_product(const Vector<T>& vector1,
                               const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Arithmetic auto result{0};
    for (size_t i{0UL}; i < vector1.size(); ++i) {
      result += vector1(i) * vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Vector<T> subtract_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector1.size());
    for (size_t i{0UL}; i < vector1.size(); ++i) {
      result(i) = vector1(i) - vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
Vector<T> add_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  auto resultFuture = std::async(std::launch::async, [&]() {
    Vector<T> result(vector1.size());
    for (size_t i{0UL}; i < vector1.size(); ++i) {
      result(i) = vector1(i) + vector2(i);
    }
    return result;
  });
  return resultFuture.get();
}

template <Arithmetic T>
bool equal_vectors(const Vector<T>& vector1, const Vector<T>& vector2) {
  if (vector1.size() != vector2.size()) {
    throw BadDimensionException("The vector dimensions have to be the same.");
  }
  bool returnValue = vector1.size() == vector2.size();
  for (size_t index{0UL}; index < vector1.size(); ++index) {
    returnValue &= vector1(index) == vector2(index);
  }
  return returnValue;
}

}  // namespace libMatrix::inline v1

#endif  // MATRIX_VECTORHELPERFUNCTIONS_HPP
