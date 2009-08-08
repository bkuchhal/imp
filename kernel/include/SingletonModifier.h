/**
 *  \file SingletonModifier.h    \brief A Modifier on Particles
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMP_SINGLETON_MODIFIER_H
#define IMP_SINGLETON_MODIFIER_H

#include "config.h"
#include "internal/version_info.h"
#include "internal/container_helpers.h"
#include "DerivativeAccumulator.h"
#include "base_types.h"
#include "VectorOfRefCounted.h"

IMP_BEGIN_NAMESPACE
// to keep swig happy
class Particle;

//! A base class for modifiers of Particles
/** The primary function of such a class is to change
    the passed particles.

    A given SingletonModifier may only work when passed a
    DerivativeAccumulator or when not passed one.

    \see IMP::SingletonFunctor

    Implementors should see IMP_SINGLETON_MODIFIER() and
    IMP_SINGLETON_MODIFIER_DA().
 */
class IMPEXPORT SingletonModifier : public Object
{
public:
  SingletonModifier(std::string name="SingletonModifier %1%");

  /** Apply the function to a single value*/
  virtual void apply(Particle *a,
                     DerivativeAccumulator &da) const {
    IMP_failure("This SingletonModifier must be called without a"
                << " DerivativeAccumulator.", InvalidStateException);
  }

  /** Apply the function to a single value*/
  virtual void apply(Particle *a) const {
    IMP_failure("This SingletonModifier must be called with a"
                << " DerivativeAccumulator.", InvalidStateException);
  }

  /** Apply the function to a collection of Particles */
  virtual void apply(const ParticlesTemp &o) const {
    IMP_failure("This SingletonModifier must be called with a"
                << " DerivativeAccumulator.", InvalidStateException);
  }

  /** Apply the function to a collection of Particles */
  virtual void apply(const ParticlesTemp &o,
                     DerivativeAccumulator &da) const {
    IMP_failure("This SingletonModifier must be called without a"
                << " DerivativeAccumulator.", InvalidStateException);
  }

  /** Print out information about the function, ending in a newline.*/
  virtual void show(std::ostream &out = std::cout) const=0;

  /** return information about the authors */
  virtual VersionInfo get_version_info() const = 0;

  IMP_REF_COUNTED_DESTRUCTOR(SingletonModifier);
};

IMP_OUTPUT_OPERATOR(SingletonModifier)


//! A collection
typedef VectorOfRefCounted<SingletonModifier*> SingletonModifiers;

//! Create a functor which can be used with build in C++ and python commands
/** For example, you can do
    \code
    std::for_each(particles.begin(), particles.end(),
                  IMP::SingletonFunctor(new IMP::core::Transform(tr)));
    IMP::for_each(particles,
                  IMP::SingletonFunctor(new IMP::core::Transform(tr)));
    \endcode
    in C++ (the second can be used with when \c particles is a temporary
    value) or
    \verbatim
    map(particles, SingletonFunctor(Transform(tr)))
    \endverbatim
    in python.

    \see IMP::SingletonModifier
 */
class SingletonFunctor {
  Pointer<const SingletonModifier> f_;
  DerivativeAccumulator *da_;
public:
  //! Store the SingletonModifier and the optional DerivativeAccumulator
  SingletonFunctor(const SingletonModifier *f): f_(f), da_(NULL){}
  SingletonFunctor(const SingletonModifier *f,
                   DerivativeAccumulator *da): f_(f), da_(da){
    IMP_check(da_, "The passed derivative accumulator should not be null.",
              InvalidStateException);
  }
  void operator()( Particle* p) const {
    if (da_) {
      IMP::internal::ContainerTraits<Particle>::apply(f_.get(), p, da_);
    } else {
      IMP::internal::ContainerTraits<Particle>::apply(f_.get(), p);
    }
  }
};


IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_MODIFIER_H */
