/**
 *  \file FilteredListSingletonContainer.h
 *  \brief Store a list of Particles filtered based on another list
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_FILTERED_LIST_SINGLETON_CONTAINER_H
#define IMPCORE_FILTERED_LIST_SINGLETON_CONTAINER_H

#include "core_exports.h"
#include "SingletonContainer.h"
#include "internal/core_version_info.h"

IMPCORE_BEGIN_NAMESPACE

// for swig


//! Store a list of Particles filtered by other lists
/** This class stores a list of Particles and a list of
    SingletonContainers with the invariant that none of the
    SingletonContainers contain any of the Particles stored.

    \note The indexes can change when particles are inserted
    as the list is maintained in sorted order.
 */
class IMPCOREEXPORT FilteredListSingletonContainer
  : public SingletonContainer
{
  std::vector<Particle*> data_;
public:
  FilteredListSingletonContainer();

  virtual ~FilteredListSingletonContainer();

  IMP_SINGLETON_CONTAINER(internal::core_version_info);

  //! Add vt if one of the referenced containers already contains it
  void add_particle(Particle* vt);

  void clear_particles() {
    data_.clear();
  }

  IMP_CONTAINER(SingletonContainer, singleton_container,
                SingletonContainerIndex);
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_FILTERED_LIST_SINGLETON_CONTAINER_H */
