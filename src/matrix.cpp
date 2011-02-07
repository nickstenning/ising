#include <iostream>
#include <iomanip>
#include <vector>

#include "matrix.h"

void Matrix::setAll(double val) {
  m_mtx = std::vector<double> (m_rows * m_cols, val);
}

void Matrix::serialize(std::ostream &os) const {
  // std::vector is guaranteed to be contiguous
  os.write(reinterpret_cast<char const*>(&(m_mtx.front())), rows() * cols() * sizeof(double));
}

std::ostream& operator<< (std::ostream& os, Matrix const& mtx) {
  for (size_t i = 0; i < mtx.rows(); i += 1) {
    for (size_t j = 0; j < mtx.cols(); j += 1) {
      os << mtx(i, j) << " ";
    }
    os << "\n";
  }
  return os;
}

std::istream& operator>> (std::istream& is, Matrix& mtx) {
  for (size_t i = 0; i < mtx.rows(); i += 1) {
    for (size_t j = 0; j < mtx.cols(); j += 1) {
      is >> mtx(i, j);
    }
  }
  return is;
}
