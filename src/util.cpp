#include <cmath>
#include <cstdlib>
#include <iostream>

#include "mersenne_twister.h"
#include "util.h"

static MTRand rng;

unsigned long util::initRNG () {
  unsigned long seed;
  char const* envSeed;

  envSeed = getenv("RNG_SEED");

  if (envSeed != NULL) {
    seed = strtoul(envSeed, NULL, 0);
  } else {
    seed = std::time(0);
  }

  rng.seed(seed);
  return seed;
}

double util::rand (double a, double b) {
  return (b - a) * rng() + a;
}

bool util::choose (double prob) {
  return (util::rand(0,1) < prob);
}