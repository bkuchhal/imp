/**
 *  \file TripletsRestraint.h
 *  \brief Apply a TripletScore to each ParticleTriplet in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPCORE_TRIPLETS_RESTRAINT_H
#define IMPCORE_TRIPLETS_RESTRAINT_H

#include "config.h"

#include <IMP/Restraint.h>
#include <IMP/Pointer.h>
#include <IMP/TripletScore.h>
#include <IMP/TripletContainer.h>

#include <iostream>

IMPCORE_BEGIN_NAMESPACE

//! Applies a TripletScore to each ParticleTriplet in a list.
/** This restraint stores the used particles in a ParticleTriplets.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListTripletContainer is used and the
    {add_, set_, clear_}particle_triplet{s} methods can be used.

    \see TripletRestraint
 */
class IMPCOREEXPORT TripletsRestraint : public Restraint
{
  IMP::internal::OwnerPointer<TripletScore> ss_;
  IMP::internal::OwnerPointer<TripletContainer> pc_;
  mutable double score_;
public:
  //! Create the restraint.
  /** \param[in] ss The function to apply to each particle.
      \param[in] ps The list of particles to use in the restraint.
      \param[in] name The object name
  */
  TripletsRestraint(TripletScore *ss,
                      const ParticleTriplets &ps=ParticleTriplets(),
                      std::string name="TripletsRestraint %1%");

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
      \param[in] name The object name
   */
  TripletsRestraint(TripletScore *ss,
                      TripletContainer *pc,
                      std::string name="TripletsRestraint %1%");

  IMP_INCREMENTAL_RESTRAINT(TripletsRestraint, get_module_version_info());

  /** \note This can only be used if the container is a ListTripletContainer
  */
  void add_particle_triplets(const ParticleTriplets &ps);
  /** \note This can only be used if the container is a ListTripletContainer
   */
  void set_particle_triplets(const ParticleTriplets &ps);
  /** \note This can only be used if the container is a ListTripletContainer
  */
  void add_particle_triplet(ParticleTriplet v);
  /** \note This can only be used if the container is a ListTripletContainer
  */
  void clear_particle_triplets();

  //! Get the container used to store Particles
  TripletContainer* get_triplet_container() {
    return pc_;
  }
  //! Set the container used to store Particles
  /** \note The passed container is used, not a copy.
   */
  void set_triplet_container(TripletContainer *pc) {
    pc_=pc;
  }

  TripletScore* get_triplet_score() const {
    return ss_;
  }

  void set_triplet_score(TripletScore* ss) {
    IMP_USAGE_CHECK(ss, "The TripletScore must be non-null",
              IndexException);
    ss_= ss;
  }
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_TRIPLETS_RESTRAINT_H */
