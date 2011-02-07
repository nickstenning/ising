#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>

/**
 * Container for util namespace.
**/
namespace util {
  /**
   * Simple helper to initialize RNG.
   *
   * `initRNG' will either seed the random number generator automatically, or
   * detect a seed in environment variable `MAW_RNG_SEED' and use that. The
   * latter can be useful for repeating runs.
  **/
  unsigned long initRNG ();

  /**
   * Normalised RNG.
   *
   * @param a lower bound.
   * @param b upper bound.
   * @return A double in [a, b)
  **/
  double rand (double a = 0, double b = 1);

  /**
   * Random boolean choice
   *
   * @param prob Probability of returning `true'
  **/
  bool choose (double prob = 0.5);
}

#endif // UTIL_H