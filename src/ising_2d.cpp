#include <cmath>

#include "periodic_matrix.h"
#include "util.h"
#include "ising_2d.h"

const double Ising2D::J = 1.0;
const double Ising2D::T = 2.0;

Ising2D::Ising2D (size_t width, size_t height)
: PeriodicMatrix<short>(width, height)
{
  reset();
}

void Ising2D::reset ()
{
  for(size_t i = 0; i < rows(); i += 1) {
    for(size_t j = 0; j < cols(); j += 1) {
      get(i, j) = util::choose(0.5) ? 1 : -1;
    }
  }
}

void Ising2D::step (size_t numSteps)
{
  while (numSteps > 0) {
    numSteps -= 1;
    physics_step();
  }
}

void Ising2D::physics_step ()
{
  for(size_t k = 0; k < rows() * cols(); k += 1) {
    size_t i = static_cast<size_t>(util::rand(0.5, rows() - 0.5));
    size_t j = static_cast<size_t>(util::rand(0.5, cols() - 0.5));

    double E_curr = - J * get(i, j) * (
      get(i + 1, j) + get(i - 1, j) + get(i, j + 1) + get(i, j - 1)
    );

    double E_flip = - J * -1 * get(i, j) * (
      get(i + 1, j) + get(i - 1, j) + get(i, j + 1) + get(i, j - 1)
    );

    if (E_flip < E_curr || util::choose(exp(-(E_flip - E_curr) / T))) {
      get(i, j) *= -1;
    }

  }
}