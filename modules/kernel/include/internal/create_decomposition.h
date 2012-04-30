/**
 *  \file internal/utility.h
 *  \brief Various useful utilities
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_INTERNAL_CREATE_DECOMPOSITION_H
#define IMPKERNEL_INTERNAL_CREATE_DECOMPOSITION_H

#include "../kernel_config.h"
#include "TupleRestraint.h"
#include "container_helpers.h"
#include <sstream>

IMP_BEGIN_INTERNAL_NAMESPACE

template <class Container, class Score>
Restraints create_current_decomposition(Model *m,Score *score,
                                        Container *c,
                                        std::string name) {
  IMP_USAGE_CHECK(m, "nullptr passed for the Model.");
  IMP_USAGE_CHECK(score, "nullptr passed for the Score.");
  Restraints ret;
  for (unsigned int i=0; i< c->get_number(); ++i) {
    std::ostringstream oss;
    oss << name << " " << Showable(c->get(i));
    Pointer<Restraint> r= IMP::internal::create_restraint(score,
                                                          c->get(i),
                             oss.str());
    double score= r->unprotected_evaluate(false);
    if (score != 0) {
      r->set_last_score(score);
      ret.push_back(r);
    } else {
      r->set_was_used(true);
    }
  };
  return ret;
}

template <class Container, class Score>
Restraints create_decomposition(Model *m,Score *score,
                                Container *c,
                                std::string name) {
  IMP_USAGE_CHECK(m, "nullptr passed for the Model.");
  IMP_USAGE_CHECK(score, "nullptr passed for the Score.");
  typename Container::ContainedIndexTypes all=c->get_all_possible_indexes();
  Restraints ret(all.size());
  for (unsigned int i=0; i< all.size(); ++i) {
    std::ostringstream oss;
    oss << name << " " << Showable(all[i]);
    ret[i]= IMP::internal::create_restraint(score,
                                             IMP::internal::get_particle(m,
                                                                       all[i]),
                                             oss.str());
  }
  return ret;
}

IMP_END_INTERNAL_NAMESPACE

#endif  /* IMPKERNEL_INTERNAL_CREATE_DECOMPOSITION_H */
