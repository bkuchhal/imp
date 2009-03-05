/**
 *  \file MaximumGroupnameScoreRestraint.h
 *  \brief Score based on the maximum score over a set of Groupnames
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_MAXIMUM_GROUPNAME_SCORE_RESTRAINT_H
#define IMPCORE_MAXIMUM_GROUPNAME_SCORE_RESTRAINT_H

#include "config.h"
#include "internal/version_info.h"
#include <IMP/Restraint.h>
#include <IMP/GroupnameScore.h>
#include <IMP/GroupnameContainer.h>


IMPCORE_BEGIN_NAMESPACE

//! Score based on the maximum over a set of Groupnames
/** The score is evaluated for each of the Groupnames in the container
 and the value of the minumum n scores is used. That is,
 if n is 1, the value of the restraint is the value of the lowest
 score over the container.
 \see MinimumGroupnameScoreRestraint
 */
class IMPCOREEXPORT MaximumGroupnameScoreRestraint
: public Restraint
{
  Pointer<GroupnameScore> f_;
  Pointer<GroupnameContainer> c_;
  unsigned int n_;
public:
  /** n is the number of minimumal scores to use.
   */
  MaximumGroupnameScoreRestraint(GroupnameScore *f,
                                 GroupnameContainer *c,
                                 unsigned int n=1);

  IMP_RESTRAINT(MaximumGroupnameScoreRestraint, internal::version_info);

  //! Set the number of lowest scores to use.
  void set_n(unsigned int n) { n_=n;}

  ParticlesList get_interacting_particles() const;
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_MAXIMUM_GROUPNAME_SCORE_RESTRAINT_H */
