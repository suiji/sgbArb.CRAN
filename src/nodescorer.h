// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
   @file nodescorer.h

   @brief Scoring methods for frontier.

   @author Mark Seligman
 */

#ifndef OBS_NODESCORER_H
#define OBS_NODESCORER_H


#include "typeparam.h"


#include <vector>
#include <numeric>
#include <algorithm>


struct NodeScorer {
  static string scoreStr; ///< Initialized per training session.

  vector<double> ctgJitter; ///< Breaks ties; frontier-wide.
  vector<double> gamma; ///< Per-sample weight, with multiplicity.

  
  double (NodeScorer::* scorer)(const struct SampleMap&,
				const class IndexSet&) const;

  NodeScorer(double (NodeScorer::* scorer_)(const struct SampleMap&,
					    const class IndexSet&) const);


  /**
     @brief Initializes scorer string once per training session.
   */
  static void init(const string& scoreStr_);


  /**
     @brief Clears scorer string.
   */
  static void deInit();


  void frontierPreamble(const class Frontier* frontier);


  /**
     @brief Makes scorer by keying off static string.

     Currently called once per grove of trained trees.
   */
  static unique_ptr<NodeScorer> makeScorer();


  double score(const struct SampleMap& smNonterm,
	       const class IndexSet& iSet) const {
    return (this->*scorer)(smNonterm, iSet);
  }


  void setGamma(vector<double> prob) {
    gamma = std::move(prob);
  }

  
  /**
     @brief Placeholder scorer.  Should never be used.
   */
  double scoreZero(const struct SampleMap& smNonterm,
		   const class IndexSet& iSet) const;

  
  /**
     @return mean reponse over node.
   */
  double scoreMean(const struct SampleMap& smNonterm,
		   const class IndexSet& iSet) const;


  /**
     @return category with jittered plurality, plus jitter.
   */
  double scorePlurality(const struct SampleMap& smNonterm,
			const class IndexSet& iSet) const;


  /**
     @return mean score weighted by per-sample p-q probabilities.
   */
  double scoreLogOdds(const struct SampleMap& smNonterm,
		      const class IndexSet& iSet) const;
};

#endif
