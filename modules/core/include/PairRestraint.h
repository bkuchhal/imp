/**
 *  \file PairRestraint.h
 *  \brief Apply a PairScore to a ParticlePair.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPCORE_PAIR_RESTRAINT_H
#define IMPCORE_PAIR_RESTRAINT_H

#include "config.h"

#include <IMP/Restraint.h>
#include <IMP/Pointer.h>
#include <IMP/PairScore.h>

#include <iostream>

IMPCORE_BEGIN_NAMESPACE

//! Applies a PairScore to a ParticlePair.
/** This restraint stores a ParticlePair.
    \see PairRestraint
 */
class IMPCOREEXPORT PairRestraint : public Restraint
{
  IMP::internal::OwnerPointer<PairScore> ss_;
  ParticlePair v_;
  mutable double score_;
public:
  //! Create the restraint.
  /** This function takes the function to apply to the
      stored Pair and the Pair.
   */
  PairRestraint(PairScore *ss,
                     const ParticlePair& vt,
                     std::string name="PairRestraint %1%");

  IMP_INCREMENTAL_RESTRAINT(PairRestraint, get_module_version_info());
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_PAIR_RESTRAINT_H */
