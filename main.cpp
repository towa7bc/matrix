#include <iostream>

#include "Matrix.hpp"
#include "Vector.hpp"

using Matrixf = libMatrix::Matrix<float>;
using Vectorf = libMatrix::Vector<float>;
using Vectord = libMatrix::Vector<double>;

int main() {
  Matrixf m1{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 3, 4};
  Matrixf m2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 4, 3);
  Matrixf res1(2, 2);
  Matrixf res2{3, 3};
  Matrixf mc1(2, 2);
  Matrixf mc2(2, 2);
  Matrixf res4(2, 2);
  Vectorf vec1{1, 2};
  Vectorf vec2(2);
  Vectorf resVec(2);
  Vectorf crossVec(3);
  Vectorf crossVec2(3);
  float res{0};
  float scalar{2};
  mc1(0, 0) = 1;
  mc1(0, 1) = 1;
  mc1(1, 0) = 3;
  mc1(1, 1) = 1;
  mc2(0, 0) = 1;
  mc2(0, 1) = 1;
  mc2(1, 0) = 1;
  mc2(1, 1) = 1;

  vec2(0) = 1;
  vec2(1) = 4;
  bool equ = vec1 != vec2;
  bool equ2 = mc1 != mc2;
  crossVec(0) = 1;
  crossVec(1) = 2;
  crossVec(2) = 3;
  crossVec2(0) = 1;
  crossVec2(1) = 3;
  crossVec2(2) = 10;
  auto res3 = mc1 * mc2 * (vec1 + vec2);
  res2 = m1 * (-5) * m2 * scalar;
  res4 = mc1 + mc2;
  auto m6 = res2;
  resVec = 2 * vec1 - vec2;
  res = vec1 * vec2;
  auto d = cross(std::move(crossVec), std::move(crossVec2));
  std::cout << res3 << '\n';
  std::cout << res2 << '\n';
  std::cout << resVec << '\n';
  std::cout << res << '\n';
  std::cout << d << '\n';
  std::cout << m6 << '\n';
  std::cout << res4 << '\n';
  std::cout << equ << '\n';
  std::cout << equ2 << '\n';

  Vectord fmt_1(3);
  fmt_1(0) = 1;
  fmt_1(1) = 4;
  fmt_1(2) = 7.2;

  fmt::print(fmt::format("{:}", fmt_1));
  std::cout << '\n';
  return 0;
}
