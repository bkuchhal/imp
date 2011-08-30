/**
 *  \file CLASSNAMERestraint.cpp
 *  \brief Implementation
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/CLASSNAMERestraint.h"
#include <IMP/internal/container_helpers.h>

#include <IMP/log.h>


IMPCORE_BEGIN_NAMESPACE

CLASSNAMERestraint
::CLASSNAMERestraint(CLASSNAMEScore *ss,
                     ARGUMENTTYPE vt,
                     std::string name):
  CLASSNAMEScoreRestraint(name),
  ss_(ss),
  v_(IMP::internal::get_index(vt))
{
}

double CLASSNAMERestraint
::unprotected_evaluate(DerivativeAccumulator *accum) const
{
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  return ss_->evaluate_index(get_model(), v_, accum);
}

double CLASSNAMERestraint
::unprotected_evaluate_if_good(DerivativeAccumulator *accum,
                               double max) const
{
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  return ss_->evaluate_if_good_index(get_model(), v_, accum, max);
}



ParticlesTemp CLASSNAMERestraint::get_input_particles() const
{
  STORAGETYPE vi= IMP::internal::get_particle(get_model(), v_);
  return IMP::internal::get_input_particles(ss_.get(),
                                            vi);
}

ContainersTemp CLASSNAMERestraint::get_input_containers() const
{
  STORAGETYPE vi= IMP::internal::get_particle(get_model(), v_);
  return IMP::internal::get_input_containers(ss_.get(),
                                             vi);
}


Restraints CLASSNAMERestraint::do_create_current_decomposition() const {
  STORAGETYPE vi= IMP::internal::get_particle(get_model(), v_);
  return ss_->create_current_decomposition(vi);
}

void CLASSNAMERestraint::do_show(std::ostream& out) const
{
  out << "score " << ss_->get_name() << std::endl;
  out << "data "
      << IMP::internal::streamable(IMP::internal::get_particle(get_model(),v_))
      << std::endl;
}

IMPCORE_END_NAMESPACE
