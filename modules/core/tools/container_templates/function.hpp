/**
 *  \file GroupnameModifier.h    \brief A Modifier on Classnames
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMP_GROUPNAME_MODIFIER_H
#define IMP_GROUPNAME_MODIFIER_H

#include "config.h"
#include "internal/kernel_version_info.h"
#include "internal/container_helpers.h"
#include "GroupnameContainer.h"
#include "DerivativeAccumulator.h"
#include "base_types.h"

IMP_BEGIN_NAMESPACE
// to keep swig happy
class Particle;

//! A base class for modifiers of Classnames
/** The primary function of such a class is to change
    the passed particles. You can use the associated GroupnameFunction
    along with \c map or \c std::for_each to easily apply a
    GroupnameModifier to a set of Classnames.

    \see IMP::GroupnameFunctor
 */
class IMPEXPORT GroupnameModifier : public RefCountedObject
{
public:
  GroupnameModifier();

  /** Apply the function to a single value*/
  virtual void apply(ClassnameArguments,
                     DerivativeAccumulator &da) const=0;

  /** Apply the function to a single value*/
  virtual void apply(ClassnameArguments) const=0;

  /** Print out information about the function, ending in a newline.*/
  virtual void show(std::ostream &out = std::cout) const=0;

  /** return information about the authors */
  virtual VersionInfo get_version_info() const = 0;

  ~GroupnameModifier(){}
};

IMP_OUTPUT_OPERATOR(GroupnameModifier)


//! A collection
typedef std::vector<GroupnameModifier*> GroupnameModifiers;

//! Create a functor which can be used with build in C++ and python commands
/** For example, you can do
    \code
    std::for_each(particles.begin(), particles.end(),
                  SingletonFunctor(new Transform(tr)));
    \endcode
    in C++ or
    \verbatim
    map(particles, SingletonFunctor(Transform(tr)))
    \endverbatim
    in python.
   
    \see IMP::GroupnameModifier
 */
class GroupnameFunctor {
  Pointer<GroupnameModifier> f_;
  DerivativeAccumulator *da_;
public:
  //! Store the GroupnameModifier and the optional DerivativeAccumulator
  GroupnameFunctor(GroupnameModifier *f,
                   DerivativeAccumulator *da=NULL): f_(f), da_(da){}

  void operator()( Value p) const {
    if (da_) {
      IMP::internal::ContainerTraits<Classname>::apply(f_.get(), p, da_);
    } else {
      IMP::internal::ContainerTraits<Classname>::apply(f_.get(), p);
    }
  }
};


//! Return a functor which can be used to call the apply method


IMP_END_NAMESPACE

#endif  /* IMP_GROUPNAME_MODIFIER_H */
