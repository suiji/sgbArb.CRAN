// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
   @file trainbridge.cc

   @brief Exportable classes and methods from the Train class.

   @author Mark Seligman
*/

#include "forestbridge.h"
#include "trainbridge.h"
#include "samplerbridge.h"
#include "leafbridge.h"
#include "sampler.h"
#include "grove.h"

// Type completion only:
#include "nodescorer.h"
#include "sampledobs.h"
#include "fetrain.h"
#include "predictorframe.h"
#include "coproc.h"

TrainBridge::TrainBridge(unique_ptr<RLEFrame> rleFrame, double autoCompress, bool enableCoproc, vector<string>& diag) : frame(make_unique<PredictorFrame>(std::move(rleFrame), autoCompress, enableCoproc, diag)) {
  ForestBridge::init(frame->getNPred());
}


TrainBridge::~TrainBridge() = default;


vector<PredictorT> TrainBridge::getPredMap() const {
  vector<PredictorT> predMap(frame->getPredMap());
  return predMap;
}


unique_ptr<TrainedChunk> TrainBridge::train(const ForestBridge& forestBridge,
					    const SamplerBridge& samplerBridge,
					    unsigned int treeOff,
					    unsigned int treeChunk,
					    const LeafBridge& leafBridge) const {
  unique_ptr<Grove> grove = samplerBridge.getSampler()->trainGrove(frame.get(),
								   forestBridge.getForest(),
								   IndexRange(treeOff, treeChunk),
								   leafBridge.getLeaf());

  return make_unique<TrainedChunk>(std::move(grove));
}


void TrainBridge::initBlock(unsigned int trainBlock) {
  Grove::initBlock(trainBlock);
}


void TrainBridge::initProb(unsigned int predFixed,
                           const vector<double> &predProb) {
  FETrain::initProb(predFixed, predProb);
}


void TrainBridge::initTree(size_t leafMax) {
  FETrain::initTree(leafMax);
}


void TrainBridge::initBooster(const string& loss, const string& scorer, double nu) {
  FETrain::initBooster(loss, scorer, nu);
}


void TrainBridge::initNodeScorer(const string& scorer) {
  FETrain::initNodeScorer(scorer);
}


void TrainBridge::initOmp(unsigned int nThread) {
  FETrain::initOmp(nThread);
}


void TrainBridge::initSplit(unsigned int minNode,
                            unsigned int totLevels,
                            double minRatio,
			    const vector<double>& feSplitQuant) {
  FETrain::initSplit(minNode, totLevels, minRatio, feSplitQuant);
}
  

void TrainBridge::initMono(const vector<double> &regMono) {
  FETrain::initMono(frame.get(), regMono);
}


void TrainBridge::deInit() {
  ForestBridge::deInit();
  FETrain::deInit();
  Grove::deInit();
}


TrainedChunk::TrainedChunk(unique_ptr<Grove> grove_) : grove(std::move(grove_)) {
}


TrainedChunk::~TrainedChunk() = default;


const vector<double>& TrainedChunk::getPredInfo() const {
  return grove->getPredInfo();
}
