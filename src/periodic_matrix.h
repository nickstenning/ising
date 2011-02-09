#ifndef PERIODIC_MATRIX_H
#define PERIODIC_MATRIX_H

#include "matrix.h"

#include <iostream>

template<typename T>
class PeriodicMatrix : public Matrix<T>
{
public:
  PeriodicMatrix(size_t rows, size_t cols);
protected:
  virtual T& get (long int row, long int col);
  virtual T  get (long int row, long int col) const;
};

template<typename T>
inline PeriodicMatrix<T>::PeriodicMatrix(size_t r, size_t c)
: Matrix<T>(r, c)
{}

template<typename T>
inline T& PeriodicMatrix<T>::get (long int row, long int col)
{
  row %= Matrix<T>::rows();
  col %= Matrix<T>::cols();

  // Deal with inconsistency in behaviour of mod with negative parameters:
  if (row < 0) row += Matrix<T>::rows();
  if (col < 0) col += Matrix<T>::cols();

  return Matrix<T>::get(row, col);
}

template<typename T>
inline T PeriodicMatrix<T>::get (long int row, long int col) const
{
  return PeriodicMatrix<T>::get(row, col);
}

#endif // PERIODIC_MATRIX_H
