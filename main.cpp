#include <iostream>
#include <type_traits>

//#include "Vector.hpp"
#include "Matrix.hpp"

int main() {
  libmatrix::Matrix<float> m1(2, 2);
  libmatrix::Matrix<float> m2(2, 2);
  libmatrix::Matrix<float> res1(2, 2);
  libmatrix::Matrix<float> res2(2, 2);
  libmatrix::Vector<float> vec1(2);
  libmatrix::Vector<float> vec2(2);
  libmatrix::Vector<float> resVec(2);
  libmatrix::Vector<float> crossVec(3);
  libmatrix::Vector<float> crossVec2(3);
  float res{0};
  float scalar{2};
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = 4;
  m2(0, 0) = 1;
  m2(0, 1) = 2;
  m2(1, 0) = 3;
  m2(1, 1) = 4;
  vec1(0) = 1;
  vec1(1) = 2;
  vec2(0) = 1;
  vec2(1) = 2;

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
  res1 = m1 * m2;
  res2 = m1 + m2;
  // res2 = m1 * scalar;
  resVec = vec1 + vec2;
  res = vec1 * vec2;
  auto d = cross(crossVec, crossVec2);
  std::cout << res2(1, 1) << '\n';
  std::cout << res2(1, 1) << '\n';
  std::cout << resVec(1) << '\n';
  std::cout << res << '\n';
  std::cout << d(0) << '\n';
  return 0;
}
