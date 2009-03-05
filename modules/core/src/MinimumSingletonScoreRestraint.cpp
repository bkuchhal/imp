/**
 *  \file MinimumSingletonScoreRestraint.cpp
 *  \brief The minimum score over a container.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/MinimumSingletonScoreRestraint.h"
#include "IMP/core/internal/MinimalSet.h"
#include <IMP/internal/container_helpers.h>


IMPCORE_BEGIN_NAMESPACE

MinimumSingletonScoreRestraint
::MinimumSingletonScoreRestraint(SingletonScore *f,
                                 SingletonContainer *c,
                                 unsigned int n): f_(f), c_(c), n_(n){
}

namespace {
  typedef internal::MinimalSet<float,
    SingletonContainer::ParticleIterator> MS;
  template <class It, class F>
  MS find_minimal_set(It b, It e, F *f, unsigned int n) {
    IMP_LOG(TERSE, "Finding lowest " << n << " of "
            << std::distance(b,e) << std::endl);
    MS bestn(n);
    for (It it= b; it != e; ++it) {
      float score= IMP::internal::ContainerTraits<Particle>
                           ::evaluate(f, *it, NULL);

      if (bestn.can_insert(score)) {
        bestn.insert(score, it);
      }
    }
    return bestn;
  }
}

Float MinimumSingletonScoreRestraint
::evaluate(DerivativeAccumulator *da) {
  MS bestn= find_minimal_set(c_->particles_begin(),
                             c_->particles_end(), f_.get(), n_);

  float score=0;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    if (da) {
      IMP::internal::ContainerTraits<Particle>::evaluate(f_.get(),
                                                     *bestn[i].second, da);
    }
    score+= bestn[i].first;
  }

  return score;
}

void MinimumSingletonScoreRestraint::show(std::ostream &out) const {
  out << "MinimumSingletonScoreRestraint over ";
  c_->show(out);
  out << " using function ";
  f_->show(out);
  out << std::endl;
}


ParticlesList MinimumSingletonScoreRestraint::get_interacting_particles() const
{
  MS bestn= find_minimal_set(c_->particles_begin(),
                             c_->particles_end(), f_.get(), n_);
  ParticlesList ret;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    Particles c= IMP::internal::ContainerTraits<Particle>
                 ::create_set(*bestn[i].second);
    ret.push_back(c);
  }

  return ret;
}
IMPCORE_END_NAMESPACE
