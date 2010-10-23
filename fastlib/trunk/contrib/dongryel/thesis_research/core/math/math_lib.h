/**
 * @file math_lib.h
 *
 * Includes all basic FASTlib non-vector math utilities.
 */

#ifndef CORE_MATH_MATH_LIB_H
#define CORE_MATH_MATH_LIB_H

#include <math.h>
#include <stdlib.h>
#include <boost/math/special_functions/binomial.hpp>

namespace core {
namespace math {

template<typename T>
T BinomialCoefficient(unsigned n, unsigned k) {
  return (n < k) ? 0 : boost::math::binomial_coefficient<double>(n, k);
}

/** Squares a number. */
template<typename T>
inline T Sqr(T v) {
  return v * v;
}

/**
 * Forces a number to be non-negative, turning negative numbers into zero.
 *
 * Avoids branching costs (yes, we've discovered measurable improvements).
 */
template<typename T>
inline double ClampNonNegative(T d) {
  return (d + fabs(d)) / 2;
}

/**
 * Forces a number to be non-positive, turning positive numbers into zero.
 *
 * Avoids branching costs (yes, we've discovered measurable improvements).
 */
template<typename T>
inline double ClampNonPositive(T d) {
  return (d - fabs(d)) / 2;
}

/**
 * Clips a number between a particular range.
 *
 * @param value the number to clip
 * @param range_min the first of the range
 * @param range_max the last of the range
 * @return max(range_min, min(range_max, d))
 */
template<typename T>
inline double ClampRange(T value, T range_min, T range_max) {
  if(value <= range_min) {
    return range_min;
  }
  else if(value >= range_max) {
    return range_max;
  }
  else {
    return value;
  }
}

/**
 * Generates a uniform random number between 0 and 1.
 */
template<typename T>
inline double Random() {
  return rand() * (1.0 / RAND_MAX);
}

/**
 * Generates a uniform random number in the specified range.
 */
template<typename T>
inline double Random(T lo, T hi) {
  return Random<T>() * (hi - lo) + lo;
}

/**
 * Generates a uniform random integer.
 */
template<typename T>
inline int RandInt(T hi_exclusive) {
  return rand() % hi_exclusive;
}

/**
 * Generates a uniform random integer.
 */
template<typename T>
inline int RandInt(T lo, T hi_exclusive) {
  return (rand() % (hi_exclusive - lo)) + lo;
}
};
};

#include "math_lib_impl.h"

namespace core {
namespace math {
/**
 * Calculates a relatively small power using template metaprogramming.
 *
 * This allows a numerator and denominator.  In the case where the
 * numerator and denominator are equal, this will not do anything, or in
 * the case where the denominator is one.
 */
template<int t_numerator, int t_denominator>
inline double Pow(double d) {
  return core::math__private::ZPowImpl<t_numerator, t_denominator>::Calculate(d);
}

/**
 * Calculates a small power of the absolute value of a number
 * using template metaprogramming.
 *
 * This allows a numerator and denominator.  In the case where the
 * numerator and denominator are equal, this will not do anything, or in
 * the case where the denominator is one.  For even powers, this will
 * avoid calling the absolute value function.
 */
template<int t_numerator, int t_denominator>
inline double PowAbs(double d) {
  // we specify whether it's an even function -- if so, we can sometimes
  // avoid the absolute value sign
  return core::math__private::ZPowAbsImpl < t_numerator, t_denominator,
         (t_numerator % t_denominator == 0) && ((t_numerator / t_denominator) % 2 == 0) >::Calculate(fabs(d));
}

template<typename T>
void RandomCombination(
  int begin, int end, int num_elements, std::vector<T> *combination) {

  for(int i = end - num_elements; i < end; i++) {
    int t = core::math::RandInt(0, i + 1);
    bool already_in_list = false;
    for(unsigned int j = 0; already_in_list == false &&
        j < combination->size(); j++) {
      already_in_list = ((*combination)[j] == t);
    }

    if(already_in_list == false) {
      combination->push_back(t);
    }
    else {
      combination->push_back(i);
    }
  }
}
};
};

#endif
