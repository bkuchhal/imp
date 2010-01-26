/**
 *  \file SingletonContainerSet.h
 *  \brief Store a set of SingletonContainers
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_SINGLETON_CONTAINER_SET_H
#define IMPCORE_SINGLETON_CONTAINER_SET_H

#include "config.h"
#include <IMP/SingletonContainer.h>
#include <IMP/container_macros.h>

IMPCORE_BEGIN_NAMESPACE

//! Stores a set of SingletonContainers
/** The input sets must be disjoint. This can change if there is
    demand for it.

    \usesconstraint
 */
class IMPCOREEXPORT SingletonContainerSet
  : public SingletonContainer
{
 // to not have added and removed
 SingletonContainerSet(bool);
public:
  //! Construct and empty set
  SingletonContainerSet(std::string name="SingletonContainerSet %1%");

  SingletonContainerSet(const SingletonContainers &in,
                        std::string name="SingletonContainerSet %1%");

  IMP_SINGLETON_CONTAINER(SingletonContainerSet, get_module_version_info());
 /** @name Methods to control the nested container

     This container merges a set of nested containers. To add
     or remove nested containers, use the methods below.
  */
  /**@{*/
  IMP_LIST(public, SingletonContainer, singleton_container,
           SingletonContainer*, SingletonContainers);
  /**@}*/

  static SingletonContainerSet *create_untracked_container() {
    SingletonContainerSet *lsc = new SingletonContainerSet(false);
    return lsc;
  }
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_SINGLETON_CONTAINER_SET_H */
