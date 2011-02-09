#ifndef ISING_2D_H
#define ISING_2D_H

#include "periodic_matrix.h"

class Ising2D : public PeriodicMatrix<short>
{
public:
  Ising2D(size_t width, size_t height);

  void reset();
  void step(size_t numSteps = 1);
  void physics_step();

  static const double J;
  static const double T;
};

#endif // ISING_2D_H
