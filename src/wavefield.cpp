#include <cmath>

#include "wavefield.h"

const double Wavefield::dt = 0.1;

Wavefield::Wavefield (size_t width, size_t height)
: Matrix(width, height)
, m_t(0.0)
{}

void Wavefield::step (double ext_dt)
{
  while (ext_dt > dt) {
    ext_dt -= dt;
    physics_step(dt);
  }
  physics_step(ext_dt);
}

void Wavefield::physics_step (double h)
{
  m_t += h;

  double o_x = rows() / 2.0;
  double o_y = cols() / 2.0;

  for(size_t i = 0; i < rows(); i += 1) {
    for(size_t j = 0; j < cols(); j += 1) {
      double r = sqrt(pow(i - o_x, 2) + pow(j - o_y, 2));
      (*this)(i, j) = sin(r - 2.0 * m_t);
    }
  }
}