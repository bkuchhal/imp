/**
 *  \file MinimumPairScoreRestraint.h
 *  \brief Score based on the minimum score over a set of Pairs
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_MINIMUM_PAIR_SCORE_RESTRAINT_H
#define IMPCORE_MINIMUM_PAIR_SCORE_RESTRAINT_H

#include "config.h"
#include "internal/version_info.h"
#include <IMP/Restraint.h>
#include <IMP/PairScore.h>
#include <IMP/PairContainer.h>

IMPCORE_BEGIN_NAMESPACE

//! Score based on the minimum over a set of Pairs
/** The score is evaluated for each of the Pairs in the container
    and the value of the minumum n scores is used. That is,
    if n is 1, the value of the restraint is the value of the lowest
    score over the container.
    \see MaximumPairScoreRestraint
 */
class IMPCOREEXPORT MinimumPairScoreRestraint
: public Restraint
{
  Pointer<PairScore> f_;
  Pointer<PairContainer> c_;
  unsigned int n_;
public:
  /** n is the number of minimumal scores to use.
   */
  MinimumPairScoreRestraint(PairScore *f,
                                 PairContainer *c,
                                 unsigned int n=1);

  IMP_RESTRAINT(MinimumPairScoreRestraint, internal::version_info);

  //! Set the number of lowest scores to use.
  void set_n(unsigned int n) { n_=n;}

  ParticlesList get_interacting_particles() const;
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_MINIMUM_PAIR_SCORE_RESTRAINT_H */
