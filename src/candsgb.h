// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SGB_CANDSGB_H
#define SGB_CANDSGB_H

/**
   @file candsgb.h

   @brief Manages SGB-specific splitting candidate selection.

   @author Mark Seligman
 */

#include "cand.h"
#include "typeparam.h"

#include <vector>

/**
   Candidate selection for Random Forest algorithm.
 */
struct CandSGB : public Cand {

  CandSGB(class InterLevel* interLevel);

  
  static void init(PredictorT feFixed,
		   const vector<double>& feProb);

  static void deInit();

  
  void precandidates(const class Frontier* frontier,
		     class InterLevel* interLevel);


private:
  // Predictor sampling paraemters.
  static PredictorT predFixed;
  static vector<double> predProb;
};

#endif
