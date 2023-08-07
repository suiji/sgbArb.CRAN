// This file is part of ArboristCore.

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
   @file algparam.h

   @brief Algorithm-specific typedefs.

   @author Mark Seligman

 */


#ifndef SGB_ALGPARAM_H
#define SGB_ALGPARAM_H

#include "candsgb.h"
#include "splitcart.h"

typedef CandSGB CandType; // SGB-specific predictor sampling.
typedef SplitCart SplitFactoryT; // CART-specific split dispatch.


#endif
