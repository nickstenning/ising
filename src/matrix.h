#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

template<typename T>
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

  T& operator() (size_t row, size_t col);
  T operator() (size_t row, size_t col) const;

  void setAll(T val);

  void serialize(std::ostream&) const;

protected:
  virtual T& get (size_t row, size_t col);
  virtual T  get (size_t row, size_t col) const;

private:
  size_t m_rows;
  size_t m_cols;
  std::vector<T> m_mtx;
};

template<typename T>
inline Matrix<T>::Matrix(size_t r, size_t c)
: m_rows(r)
, m_cols(c)
, m_mtx(r * c)
{}

template<typename T>
inline size_t Matrix<T>::rows() const
{
  return m_rows;
}

template<typename T>
inline size_t Matrix<T>::cols() const
{
  return m_cols;
}

template<typename T>
inline T& Matrix<T>::get (size_t row, size_t col)
{
  if (row >= m_rows || col >= m_cols) throw BadIndex();
  return m_mtx[m_cols * row + col];
}

template<typename T>
inline T Matrix<T>::get (size_t row, size_t col) const
{
  if (row >= m_rows || col >= m_cols) throw BadIndex();
  return m_mtx[m_cols * row + col];
}

template<typename T>
inline T& Matrix<T>::operator() (size_t row, size_t col)
{
  return get(row, col);
}

template<typename T>
inline T Matrix<T>::operator() (size_t row, size_t col) const
{
  return get(row, col);
}

template<typename T>
void Matrix<T>::setAll(T val) {
  m_mtx = std::vector<T> (m_rows * m_cols, val);
}

template<typename T>
void Matrix<T>::serialize(std::ostream &os) const {
  // std::vector is guaranteed to be contiguous
  os.write(reinterpret_cast<char const*>(&(m_mtx.front())), rows() * cols() * sizeof(T));
}

template<typename T>
std::ostream& operator<< (std::ostream& os, Matrix<T> const& mtx) {
  for (size_t i = 0; i < mtx.rows(); i += 1) {
    for (size_t j = 0; j < mtx.cols(); j += 1) {
      os << mtx(i, j) << " ";
    }
    os << "\n";
  }
  return os;
}

template<typename T>
std::istream& operator>> (std::istream& is, Matrix<T>& mtx) {
  for (size_t i = 0; i < mtx.rows(); i += 1) {
    for (size_t j = 0; j < mtx.cols(); j += 1) {
      is >> mtx(i, j);
    }
  }
  return is;
}
#endif // MATRIX_H
