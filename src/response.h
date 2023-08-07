// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
   @file response.h

   @brief Scoring methods parametrized by response type.

   @author Mark Seligman
 */

#ifndef OBS_RESPONSE_H
#define OBS_RESPONSE_H


#include "typeparam.h"

#include <vector>
#include <numeric>
#include <algorithm>

/**
   @brief Abstract wrapper class.  Probably unnecessary.
 */
struct Response {
  Response() = default;

  virtual ~Response() {}


  virtual PredictorT getNCtg() const = 0;

  
  /**
     @brief Temporary workaround for singleton base score in Scorer.
   */
  virtual double getDefaultPrediction() const = 0;

  /**
     @base Copies front-end vectors and lights off initializations specific to classification.

     @param yCtg is the zero-indexed response vector.

     @return void.
  */
  static unique_ptr<class ResponseCtg> factoryCtg(const vector<unsigned int>& yCtg,
					      PredictorT nCtg,
					      const vector<double>& classWeight);

  
  static unique_ptr<class ResponseCtg> factoryCtg(const vector<unsigned int>& yCtg,
					      PredictorT nCtg);

    
  static unique_ptr<class ResponseReg> factoryReg(const vector<double>& yNum);

  
  /**
     @brief Samples (bags) the estimand to construct the tree root.
   */
  virtual class SampledObs* getObs(const class Sampler* sampler,
				   unsigned int tIdx) const = 0;
};


class ResponseReg : public Response {
  const vector<double> yTrain; ///< Training response.

  const double defaultPrediction; ///< Prediction value when no trees bagged.


  /**
     @brief Determines mean training value.

     @return mean trainig value.
   */
  double meanTrain() const {
    return yTrain.empty() ? 0.0 : accumulate(yTrain.begin(), yTrain.end(), 0.0) / yTrain.size();
  }


public:
  /**
     @brief Regression constructor.

     @param y is the training response.
   */
  ResponseReg(const vector<double>& y);


  ~ResponseReg() = default;

  
  PredictorT getNCtg() const {
    return 0;
  }


  /**
     @brief Temporary workaround.
   */
  double getDefaultPrediction() const;


  const vector<double>& getYTrain() const {
    return yTrain;
  }
  

  /**
     @brief Samples response of current tree.

     @return summary of sampled response.
   */
  class SampledObs* getObs(const class Sampler* sampler,
			   unsigned int tIdx) const;
};


/**
   @brief Training members and methods for categorical response.
 */
class ResponseCtg : public Response {
  const vector<PredictorT> yCtg; ///< 0-based factor-valued response.
  const PredictorT nCtg;
  const vector<double> classWeight; ///< Category weights:  cresecent only.
  const PredictorT defaultPrediction; ///< Default prediction when nothing is out-of-bag.


  /**
     @return highest probability category of default vector.
  */
  PredictorT ctgDefault() const;


public:
  /**
     @breif Training constructor:  class weights needed.
   */
  ResponseCtg(const vector<PredictorT>& yCtg_,
	      PredictorT nCtg,
	      const vector<double>& classWeight);


  /**
     @brief Post-training constructor.
   */
  ResponseCtg(const vector<PredictorT>& yCtg_,
	      PredictorT nCtg);


  ~ResponseCtg() = default;


  const vector<double>& getClassWeight() const {
    return classWeight;
  }


  const vector<unsigned int>& getYCtg() const {
    return yCtg;
  }

  
  inline auto getCtg(IndexT row) const {
    return yCtg[row];
  }


  PredictorT getNCtg() const {
    return nCtg;
  }


  /**
   */
  double getDefaultPrediction() const;
  

  /**
     @brief Samples training response of current tree.

     @return summary of sampled response.
   */
  class SampledObs* getObs(const class Sampler* sampler,
			   unsigned int tIdx) const;


  /**
     @brief Constructs a vector of default probabilities.
  */
  vector<double> defaultProb() const;
};

#endif
