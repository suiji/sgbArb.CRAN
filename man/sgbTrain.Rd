% File man/sgbTrain.Rd
% Part of the Rborist package

\name{sgbTrain}
\alias{sgbTrain}
\alias{sgbTrain.default}
\concept{decision trees}
\title{Rapid Training for Stochasitic Gradient Boosting}
\description{
  Accelerated training using Friedman's stochastic gradient boosting
  algorithm.  Tuned for multicore and GPU hardware.  Bindable with most
  numerical front-end languages in addtion to R.
}

\usage{
\method{sgbTrain}{default}(preFormat, sampler, y,
                autoCompress = 0.25,
                maxLeaf = 0,
                minInfo = 0.01,
                minNode = if (is.factor(y)) 2 else 3,
                nLevel = 6,
                nThread = 0,
                nTree = 500,
                nu = 0.1,
                predFixed = 0,
                predProb = 0.0,
                predWeight = NULL, 
                regMono = NULL,
                splitQuant = NULL,
                thinLeaves = FALSE,
                treeBlock = 1,
                verbose = FALSE,
                ...)
}

\arguments{
  \item{y}{ the response (outcome) vector, either numerical or
    categorical.}
  \item{preFormat}{Compressed, presorted representation of the predictor
    values. Row count must conform with \code{y}.}
  \item{sampler}{Compressed representation of the sampled response.}
  \item{autoCompress}{plurality above which to compress predictor values.}
  \item{maxLeaf}{maximum number of leaves in a tree.  Zero denotes no limit.}
  \item{minInfo}{information ratio with parent below which node does not split.}
  \item{minNode}{minimum number of distinct row references to split a node.}
  \item{nLevel}{maximum number of tree levels to train, including
    terminals (leaves).  Zero denotes no limit.}
  \item{nThread}{suggests an OpenMP-style thread count.  Zero denotes
    the default processor setting.}
  \item{nTree}{is the number of trees to train.}
  \item{nu}{is the learning rate.}
  \item{predFixed}{number of trial predictors for a split (\code{mtry}).}
  \item{predProb}{probability of selecting individual predictor as trial splitter.}
  \item{predWeight}{relative weighting of individual predictors as trial
    splitters.}
  \item{regMono}{signed probability constraint for monotonic
    regression.}
  \item{splitQuant}{(sub)quantile at which to place cut point for
    numerical splits}.
  \item{thinLeaves}{bypasses creation of leaf state in order to reduce
    memory footprint.}
  \item{treeBlock}{maximum number of trees to train during a single
    level (e.g., coprocessor computing).}
  \item{verbose}{indicates whether to output progress of training.}
  \item{...}{Not currently used.}
}

\value{an object of class \code{trainArb}, containing:
  \item{version}{The version of the \code{Rborist} package used to train.}
  \item{samplerHash}{Hash value of the \code{Sampler} object used to
      train.  Recorded for consistency of subsequent commands.}
  \item{predInfo}{A vector of forest-wide Gini (classification) or weighted
    variance (regression), by predictor.}
  \item{predMap}{A vector of integers mapping internal to front-end
    predictor indices.}
  \item{forest}{an object of class \code{Forest} containing:
    \code{nTree}{The number of trees trained.}
    \code{node}{An object of class \code{Node} consisting of:
      \code{treeNode}{Forest-wide vector of packed node representations.}
      \code{extent}{Per-tree node counts.}
    }
    \code{scores}{Numeric Vector of scores, per node.}
    \code{factor}{An object of class \code{Factor} consisting of:
      \code{facSplit}{Forest-wide vector of packed factor bits.}
      \code{extent}{Per-tree extent of factor bits.}
      \code{observed}{Forest-wide vector of observed factor bits.}
    }
  }
  \code{Leaf}{an object of class \code{Leaf} containint:
    \code{extent}{forest-wide vector of leaf populations, i.e., counts
  of unique samples.}
    \code{index}{forest-wide vector of sample indices.}
  }
  \item{diag}{Diagnostics accumulated over the training task.}
}


\examples{
\dontrun{
  # Regression example:
  nRow <- 5000
  x <- data.frame(replicate(6, rnorm(nRow)))
  y <- with(x, X1^2 + sin(X2) + X3 * X4) # courtesy of S. Welling.

  # Generic invocation:
  rt <- sgbTrain(x, y)


  # Causes 300 trees to be trained:
  rt <- sgbTrain(x, y, nTree = 300)

  # Specifies a learning rate of 0.05:
  rt <- sgbTrain(x, y, nu=0.05)

  # Does not split nodes when doing so yields less than a 2\% gain in
  # information over the parent node:
  rt <- sgbTrain(x, y, preFormat, sampler, minInfo=0.02)


  # Does not split nodes representing fewer than 10 unique samples:
  rt <- sgbTrain(x, y, preFormat, sampler, minNode=10)


  # Trains a maximum of 20 levels:
  rt <- sgbTrain(x, y, preFormat, sampler, nLevel = 20)


  # Trains, but does not perform subsequent validation:
  rt <- sgbTrain(x, y, preFormat, sampler, noValidate=TRUE)


  # Chooses 500 rows (with replacement) to root each tree.
  rt <- sgbTrain(x, y, preFormat, sampler, nSamp=500)


  # Chooses 2 predictors as splitting candidates at each node (or
  # fewer, when choices exhausted):
  rt <- sgbTrain(x, y, preFormat, sampler, predFixed = 2)  


  # Causes each predictor to be selected as a splitting candidate with
  # distribution Bernoulli(0.3):
  rt <- sgbTrain(x, y, preFormat, sampler, predProb = 0.3) 


  # Causes first three predictors to be selected as splitting candidates
  # twice as often as the other two:
  rt <- sgbTrain(x, y, preFormat, sampler, predWeight=c(2.0, 2.0, 2.0, 1.0, 1.0))


  # Constrains modelled response to be increasing with respect to X1
  # and decreasing with respect to X5.
  rt <- sgbTrain(x, y, preFormat, sampler, regMono=c(1.0, 0, 0, 0, -1.0, 0))


  # Suppresses creation of detailed leaf information needed for
  # external tools.
  rt <- sgbTrain(x, y, preFormat, sampler, thinLeaves = TRUE)

  # Sets splitting position for predictor 0 to far left and predictor
  # 1 to far right, others to default (median) position.

  spq <- rep(0.5, ncol(x))
  spq[0] <- 0.0
  spq[1] <- 1.0
  rb <- sgbArb(x, y, splitQuant = spq)
  }
}


\author{
  Mark Seligman at Suiji.
}


\seealso{\code{\link{sgbArb}}}
