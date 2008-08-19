/**
 *  \file TripletChainRestraint.h   
 *   \brief Restraint triplets of particles in chains.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef __IMP_TRIPLET_CHAIN_RESTRAINT_H
#define __IMP_TRIPLET_CHAIN_RESTRAINT_H

#include "../IMP_config.h"
#include "../Restraint.h"
#include "../internal/kernel_version_info.h"
#include "../Pointer.h"
#include "../TripletScore.h"

#include <vector>

namespace IMP
{

//! Restrain each triplet of consecutive particles in each chain.
/** \ingroup restraint
 */
class IMPDLLEXPORT TripletChainRestraint : public Restraint
{
public:
  //! Create the triplet restraint.
  /** \param[in] trip_score Triplet score to apply.
   */
  TripletChainRestraint(TripletScore* trip_score);
  virtual ~TripletChainRestraint(){}

  virtual ParticlesList get_interacting_particles() const;

  IMP_RESTRAINT(internal::kernel_version_info)

  //! Add a chain of particles
  /** Each three successive particles are restrained.
   */
  void add_chain(const Particles &ps);

  //! Clear all the stored chains
  void clear_chains();

protected:
  Pointer<TripletScore> ts_;
  std::vector<unsigned int> chain_splits_;
};

} // namespace IMP

#endif  /* __IMP_TRIPLET_CHAIN_RESTRAINT_H */
