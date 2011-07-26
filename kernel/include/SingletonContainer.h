/**
 *  \file SingletonContainer.h    \brief A container for Singletons.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMP_SINGLETON_CONTAINER_H
#define IMP_SINGLETON_CONTAINER_H

#include "kernel_config.h"
#include "internal/IndexingIterator.h"
#include "Particle.h"
#include "container_base.h"
#include "VersionInfo.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "ParticleTuple.h"
#include "SingletonScore.h"
#include "SingletonModifier.h"
#include "SingletonDerivativeModifier.h"
#include "macros.h"

IMP_BEGIN_NAMESPACE

class SingletonContainer;
typedef std::pair<SingletonContainer*,
                  SingletonContainer*> SingletonContainerPair;

//! A shared container for Singletons
/** Stores a searchable shared collection of Singletons.
    \ingroup restraints

    \implementationwithoutexample{SingletonContainer, IMP_SINGLETON_CONTAINER}
 */
class IMPEXPORT SingletonContainer : public Container
{
  mutable internal::OwnerPointer<Container> added_, removed_;
  struct Accessor {
    typedef Particle* result_type;
    typedef unsigned int argument_type;
    result_type operator()(argument_type i) const {
      return o_->get_particle(i);
    }
    Accessor(SingletonContainer *pc): o_(pc){}
    Accessor(): o_(NULL){}
    IMP_COMPARISONS_1(Accessor, o_);
  private:
    // This should be ref counted, but swig memory management is broken
    SingletonContainer* o_;
  };
 protected:
  /** Containers must have containers that keep track of the particles
      which have been added or since the last step. These containers
      must be registered with the parent SingletonContainer.

      Containers which are themselves returned by the get_added/removed
      functions do not have to register such containers.
  */
  virtual SingletonContainerPair
    get_added_and_removed_containers() const =0;
  bool get_has_added_and_removed_containers() const {
    return (added_ && added_->get_is_shared())
      || (removed_ && removed_->get_is_shared());
  }
  SingletonContainer(){}
  SingletonContainer(Model *m,
                     std::string name="SingletonContainer %1%");
#ifndef IMP_DOXYGEN
  template <class S>
    double call_evaluate_index(const S *s,
                         ParticleIndex a,
                         DerivativeAccumulator *da) const {
    return s->S::evaluate_index(get_model(), a, da);
  }
  double call_evaluate_index(const SingletonScore *s,
                              ParticleIndex a,
                              DerivativeAccumulator *da) const {
    return s->evaluate_index(get_model(), a, da);
  }
  template <class S>
    double call_evaluate_if_good_index(const S *s,
                                 ParticleIndex a,
                                 DerivativeAccumulator *da,
                                 double max) const {
    return s->S::evaluate_if_good_index(get_model(), a, da, max);
  }
  double call_evaluate_if_good_index(const SingletonScore *s,
                                      ParticleIndex a,
                                      DerivativeAccumulator *da,
                                      double max) const {
    return s->evaluate_if_good_index(get_model(), a, da, max);
  }
  template <class S>
    void call_apply_index(const S *s,
                    ParticleIndex a) const {
    s->S::apply_index(get_model(), a);
  }
  void call_apply(const SingletonModifier *s,
                         ParticleIndex a) const {
    s->apply_index(get_model(), a);
  }
  template <class S>
    void call_apply_index(const S *s,
                           ParticleIndex a,
                           DerivativeAccumulator *&da) const {
    s->S::apply_index(get_model(), a, da);
  }
  void call_apply_index(const SingletonDerivativeModifier *s,
                  ParticleIndex a,
                  DerivativeAccumulator &da) const {
    s->apply_index(get_model(), a, da);
  }
#endif
public:
  typedef Particle* ContainedType;
  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle(Particle* v) const =0;
  //! return the number of Singletons in the container
  /** \note this isn't always constant time
   */
  virtual unsigned int get_number_of_particles() const =0;

  ParticlesTemp get_particles() const {
    return ParticlesTemp(particles_begin(),
                              particles_end());
  }
  virtual Particle* get_particle(unsigned int i) const=0;

#ifdef IMP_DOXYGEN
  //! An iterator through the contents of the container
  class ParticleIterator;
#else
  typedef internal::IndexingIterator<Accessor> ParticleIterator;
#endif
#ifndef SWIG
  //! begin iterating through the Singletons
  ParticleIterator particles_begin() const {
    // Since I can't make the count mutable in Object
    return
      ParticleIterator(Accessor(const_cast<SingletonContainer*>(this)),
                        0);
  }
  //! iterate through the Singletons
  ParticleIterator particles_end() const {
    return
      ParticleIterator(Accessor(const_cast<SingletonContainer*>(this)),
                        get_number_of_particles());
    }
#endif

  //! Apply a SingletonModifier to the contents
  virtual void apply(const SingletonModifier *sm)=0;

  //! Apply a SingletonModifier to the contents
  virtual void apply(const SingletonDerivativeModifier *sm,
                     DerivativeAccumulator &da)=0;

  //! Evaluate a score on the contents
  virtual double evaluate(const SingletonScore *s,
                          DerivativeAccumulator *da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate_if_good(const SingletonScore *s,
                                  DerivativeAccumulator *da,
                                  double max) const=0;


  /** \name Tracking changes
      The container can keep track of the changes since the last
      Model::evaluate() call. To use this, make sure to call
      get_removed_container() or get_added_container() before
      Model::evaluate() so the container knows to track changes.
      @{
  */
  SingletonContainer* get_removed_container() const {
    // must not be an added or removed container
    get_model();
    if (!added_) {
      std::pair<SingletonContainer*, SingletonContainer*>
        cp= get_added_and_removed_containers();
      added_=cp.first;
      removed_=cp.second;
    }
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_container() do not "
                    << " track their own added and removed contents.");
    SingletonContainer *ret= dynamic_cast<SingletonContainer*>(removed_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << removed_->get_name()
                       << " to a SingletonContainer.");
    return ret;
  }
  SingletonContainer* get_added_container() const {
    // must not be an added or removed container
    if (!added_) {
      std::pair<SingletonContainer*, SingletonContainer*>
        cp= get_added_and_removed_containers();
      added_=cp.first;
      removed_=cp.second;
    }
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_container() do not "
                    << " track their own added and removed contents.");
    SingletonContainer *ret= dynamic_cast<SingletonContainer*>(added_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << added_->get_name()
                       << " to a SingletonContainer.");
    return ret;
  }
  /** @} */

#ifndef IMP_DOXYGEN
  typedef Particle* value_type;
  Particle* get(unsigned int i) const {return get_particle(i);}
  ParticlesTemp get() const {
    return get_particles();
  }
  bool get_contains(Particle* v) const {
    return get_contains_particle(v);
  }
  unsigned int get_number() const {return get_number_of_particles();}
#ifndef SWIG
  virtual bool get_provides_access() const {return false;}
  virtual const ParticleIndexes& get_access() const {
    IMP_THROW("Object not implemented properly.", IndexException);
  }
#endif
  virtual ParticleIndexes get_indexes() const {
    ParticleIndexes ret(get_number());
    for (unsigned int i=0; i< ret.size(); ++i) {
      ret[i]= IMP::internal::get_index(get(i));
    }
    return ret;
  }
#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(SingletonContainer);
};

IMP_OBJECTS(SingletonContainer,SingletonContainers);

IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_CONTAINER_H */
