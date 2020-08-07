#include <iostream>
#include <string>
#include <type_traits>

#include "Matrix.hpp"
#include "Vector.hpp"
//#include "Vec.hpp"

/*int main() {
  Vector v0 = {-23, 12, 144, 90};
  Vector v1{-67, 34, 90, 89};
  Vector v2{-34, 111, 45, 90};

  // Following assignment will call the ctor of Vec which accept type of
  // `VecExpression<E> const&`. Then expand the loop body to
  // a.elems[i] + b.elems[i] + c.elems[i]
  Vector sum_of_vec_type = v0 + v1 + v2;

  for (size_t i = 0; i < sum_of_vec_type.size(); ++i) {
    std::cout << sum_of_vec_type(i) << '\n';
  }

  std::cout << '\n';
  // To avoid creating any extra storage, other than v0, v1, v2
  // one can do the following (Tested with C++11 on GCC 5.3.0)
  auto sum = v0 + v1 - v2 + v0;
  for (size_t i = 0; i < sum.size(); ++i) {
    std::cout << sum(i) << '\n';
  }
  // Observe that in this case typeid(sum) will be VecSum<VecSum<Vec, Vec>, Vec>
  // and this chaining of operations can go on.
}
*/
int main() {
  libMatrix::Matrix<float> m1{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 3, 4};
  libMatrix::Matrix<float> m2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 4, 3);
  // libMatrix::Matrix<float> m1(3, 4);
  // libMatrix::Matrix<float> m2(4, 3);
  libMatrix::Matrix<float> res1(2, 2);
  libMatrix::Matrix<float> res2{3, 3};
  // libMatrix::Matrix<bool> res3(2, 2);
  libMatrix::Matrix<float> mc1(2, 2);
  libMatrix::Matrix<float> mc2(2, 2);
  libMatrix::Matrix<float> res4(2, 2);

  libMatrix::Vector<float> vec1{1, 2};
  libMatrix::Vector<float> vec2(2);
  libMatrix::Vector<float> resVec(2);
  libMatrix::Vector<float> crossVec(3);
  libMatrix::Vector<float> crossVec2(3);
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

  /* m1(0, 0) = 1;
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
 */
  // vec1(0) = 1;
  // vec1(1) = 2;
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
  // res2(0, 0) = 0;
  // res2(0, 1) = 0;
  // res2(1, 0) = 0;
  // res2(1, 1) = 0;
  // resVec = vec1 * m1;
  auto res3 = 6 * m1 * m2;
  res2 = m1 * (-5) * m2 * scalar;
  res4 = mc1 + mc2;
  libMatrix::Matrix<float> m6 = libMatrix::Matrix<float>(res2);
  resVec = 2 * vec1 - vec2;
  res = vec1 * vec2;
  auto d = cross(std::move(crossVec), std::move(crossVec2));
  // std::cout << std::move(res3) << '\n';
  std::cout << res3 << '\n';
  std::cout << res2 << '\n';
  std::cout << resVec << '\n';
  std::cout << res << '\n';
  std::cout << d << '\n';
  std::cout << m6 << '\n';
  std::cout << res4 << '\n';
  std::cout << equ << '\n';
  std::cout << equ2 << '\n';
  return 0;
}
