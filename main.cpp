#include <iostream>

#include "Matrix.hpp"
#include "Vector.hpp"

using Matrixf = libMatrix::Matrix<float>;

int main() {
  Matrixf m1{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 3, 4};
  libMatrix::Matrix<float> m2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 4, 3);
  libMatrix::Matrix<float> res1(2, 2);
  libMatrix::Matrix<float> res2{3, 3};
  libMatrix::Matrix<float> mc1(2, 2);
  libMatrix::Matrix<float> mc2(2, 2);
  libMatrix::Matrix<float> res4(2, 2);

  libMatrix::Vector<float> vec1{1, 2};
  libMatrix::Vector<float> vec2(2);
  libMatrix::Vector<float> resVec(2);
  libMatrix::Vector<float> crossVec(3);
  libMatrix::Vector<float> crossVec2(3);
//  float res{0};
//  float scalar{2};
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
  //auto res3 = mc1 * mc2 * (vec1 + vec2);
  //res2 = m1 * (-5) * m2 * scalar;
  //res4 = mc1 + mc2;
  //auto m6 = res2;
  //resVec = 2 * vec1 - vec2;
  //res = vec1 * vec2;
  //auto d = cross(std::move(crossVec), std::move(crossVec2));
  // std::cout << std::move(res3) << '\n';
  //std::cout << res3 << '\n';
  //std::cout << res2 << '\n';
  //std::cout << resVec << '\n';
  //std::cout << res << '\n';
  //std::cout << d << '\n';
  //std::cout << m6 << '\n';
  //std::cout << res4 << '\n';
  std::cout << equ << '\n';
  std::cout << equ2 << '\n';

  libMatrix::Vector<double> fmt_1(3);
  fmt_1(0) = 1;
  fmt_1(1) = 4;
  fmt_1(2) = 7.2;

  fmt::print(fmt::format("{:}", fmt_1));
  std::cout << '\n';
  return 0;
}
