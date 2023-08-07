// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
   @file typeparam.h

   @brief Definitions for parameterization of internal types and classes.

   @author Mark Seligman

 */


#ifndef CORE_TYPEPARAM_H
#define CORE_TYPEPARAM_H

#include <memory>
#include <utility>
#include <cmath>

using namespace std;

// Type for caching front-end values, but not necessarily for arithmetic.
typedef float FltVal;

// Floating accumulator type, viz. arithmetic.
typedef double FltAccum;


// Wide container type for packed values.
typedef uint64_t PackedT;

// Index type:  rows, samples, ranks, run counts.
// Should be wide enough to accommodate values approaching #
// observations.
//
// Can be set to size_t for observation counts > 32 bits, but Rcpp's
// sampling methods do not currently accommodate such large sizes.
// Setting to size_t may also incur performance penalties, roughly
// 5% more memory usage and 10% reduction in speed.
//
typedef unsigned int IndexT; 

// Predictor type:  # columns.
// Should accommodate values approaching # predictors.
typedef unsigned int PredictorT;


// Category cardinalities:  under construction.
typedef unsigned int CtgT;


// Low/extent pair defining range of indices.
struct IndexRange {
  IndexT idxStart;
  IndexT idxExtent;


  IndexRange() :
    idxStart(0),
    idxExtent(0) {
  }

  IndexRange(IndexT idxStart_,
	     IndexT idxExtent_) :
    idxStart(idxStart_),
    idxExtent(idxExtent_) {
  }


  /**
     @brief Tests for uninitialized range.

     @return true iff extent has value zero.
   */
  inline bool empty() const {
    return idxExtent == 0;
  }


  /**
     @brief Decrements bounds incurred through sparsification.
   */
  void adjust(IndexT margin,
              IndexT implicit) {
    idxStart -= margin;
    idxExtent -= implicit;
  }


  IndexT getStart() const {
    return idxStart;
  }
  

  
  IndexT getExtent() const {
    return idxExtent;
  }


  /**
     @brief Computes iterator-style end position.

     @return end position.
   */
  IndexT getEnd() const {
    return idxStart + idxExtent;
  }


  /**
     @brief Interpolates an intermediate position.

     @param scale should lie in [0.0, 1.0].

     @return fractional scaled position.
   */
  double interpolate(double scale) const {
    return idxStart + scale * idxExtent;
  }
};


typedef unsigned char PathT;

/**
   @brief Template parametrization; specialization for double.
 */
template<typename tn>
bool areEqual(const tn& val1,
	      const tn& val2) {
  return val1 == val2;
}


/**
   @brief Double override to check for NaN.
 */
inline bool areEqual(const double& val1,
	      const double& val2) {
  return ((val1 == val2) || (isnan(val1) && isnan(val2)));
}

  
#endif
