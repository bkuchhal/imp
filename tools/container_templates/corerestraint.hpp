/**
 *  \file CoreCLASSNAMEsRestraint.h
 *  \brief Apply a CLASSNAMEScore to each CLASSNAME in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCORE_INTERNAL_CORE_HEADERNAMES_RESTRAINT_H
#define IMPCORE_INTERNAL_CORE_HEADERNAMES_RESTRAINT_H

#include "../core_config.h"

#include <IMP/Pointer.h>
#include <IMP/CLASSNAMEScore.h>
#include <IMP/CLASSNAMEContainer.h>
#include "HELPERNAME_helpers.h"

#include <iostream>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

//! Applies a CLASSNAMEScore to each CLASSNAME in a list.
/** This restraint stores the used particles in a PLURALVARIABLETYPE.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListCLASSNAMEContainer is used and the
    {add_, set_, clear_}FUNCTIONNAME{s} methods can be used.

    \see CLASSNAMERestraint
 */
class IMPCOREEXPORT CoreCLASSNAMEsRestraint :
  public CLASSNAMEsScoreRestraint
{
  IMP::internal::OwnerPointer<CLASSNAMEScore> ss_;
  IMP::internal::OwnerPointer<CLASSNAMEContainer> pc_, ac_, rc_;
  mutable double score_;
public:

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
      \param[in] name The object name
   */
  CoreCLASSNAMEsRestraint(CLASSNAMEScore *ss,
                      CLASSNAMEContainer *pc,
                      std::string name="CLASSNAMEsRestraint %1%");

  IMP_INCREMENTAL_RESTRAINT(CoreCLASSNAMEsRestraint);

  PLURALVARIABLETYPE get_arguments() const {
    return pc_->get();
  }

  CLASSNAMEScore* get_score() const {
    return ss_;
  }

  CLASSNAMEContainer* get_container() const {
    return pc_;
  }

  Restraints get_decomposition() const;

  Restraints get_instant_decomposition() const;
};

IMPCORE_END_INTERNAL_NAMESPACE

#endif  /* IMPCORE_INTERNAL_CORE_HEADERNAMES_RESTRAINT_H */
