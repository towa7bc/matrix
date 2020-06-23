#include <iostream>
#include <type_traits>

//#include "Vector.hpp"
#include "Matrix.hpp"

int main() {
  libMatrix::Matrix<float> m1(3, 4);
  libMatrix::Matrix<float> m2(4, 3);
  libMatrix::Matrix<float> res1(2, 2);
  libMatrix::Matrix<float> res2(3, 3);
  libMatrix::Matrix<float> res3(2, 2);

  libMatrix::Vector<float> vec1(2);
  libMatrix::Vector<float> vec2(2);
  libMatrix::Vector<float> resVec(2);
  libMatrix::Vector<float> crossVec(3);
  libMatrix::Vector<float> crossVec2(3);
  float res{0};
  float scalar{2};
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = 4;
  m1(1, 2) = 5;
  m1(2, 1) = 6;
  m1(2, 2) = 7;
  m2(0, 0) = 1;
  m2(0, 1) = 2;
  m2(1, 0) = 3;
  m2(1, 1) = 4;
  m2(1, 2) = 5;
  m2(2, 1) = 6;
  m2(2, 2) = 7;

  vec1(0) = 1;
  vec1(1) = 2;
  vec2(0) = 1;
  vec2(1) = 3;

  crossVec(0) = 1;
  crossVec(1) = 2;
  crossVec(2) = 3;
  crossVec2(0) = 1;
  crossVec2(1) = 3;
  crossVec2(2) = 10;
  // res2(0, 0) = 0;
  // res2(0, 1) = 0;
  // res2(1, 0) = 0;
  // res2(1, 1) = 0;
  // resVec = vec1 * m1;
  // res3 = 5 * m1 + 2 * m2;
  res2 = m1 * (-5) * m2 * scalar;

  libMatrix::Matrix<float> m6 = libMatrix::Matrix<float>(res2);
  resVec = 2 * vec1 - vec2;
  res = vec1 * vec2;
  auto d = cross(std::move(crossVec), std::move(crossVec2));
  // std::cout << std::move(res3) << '\n';
  std::cout << res2 << '\n';
  std::cout << resVec << '\n';
  std::cout << res << '\n';
  std::cout << d << '\n';
  std::cout << m6 << '\n';
  return 0;
}
