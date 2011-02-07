#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix
{
  class BadIndex : public std::runtime_error {
  public:
    BadIndex() : std::runtime_error("Index out of range") {}
  };

public:
  Matrix(size_t rows, size_t cols);
  virtual ~Matrix() {};

  size_t rows() const;
  size_t cols() const;

  double& operator() (size_t row, size_t col);
  double operator() (size_t row, size_t col) const;

  void setAll(double val);

  void serialize(std::ostream&) const;
private:
  size_t m_rows;
  size_t m_cols;
  std::vector<double> m_mtx;
};

inline Matrix::Matrix(size_t r, size_t c)
: m_rows(r)
, m_cols(c)
, m_mtx(r * c)
{}

inline size_t Matrix::rows() const
{
  return m_rows;
}

inline size_t Matrix::cols() const
{
  return m_cols;
}

inline double& Matrix::operator() (size_t row, size_t col)
{
  if (row >= m_rows || col >= m_cols) throw BadIndex();
  return m_mtx[m_cols * row + col];
}

inline double Matrix::operator() (size_t row, size_t col) const
{
  if (row >= m_rows || col >= m_cols) throw BadIndex();
  return m_mtx[m_cols * row + col];
}

std::ostream& operator<< (std::ostream& os, Matrix const& mtx);
std::istream& operator>> (std::istream& os, Matrix& mtx);

#endif // MATRIX_H
