#ifndef WAVEFIELD_H
#define WAVEFIELD_H

#include "matrix.h"

class Wavefield : public Matrix
{
public:
  Wavefield(size_t width, size_t height);

  void step(double ext_dt = Wavefield::dt);
  void physics_step(double dt);

  static const double dt;
private:
  double m_t;
};

#endif // WAVEFIELD_H
