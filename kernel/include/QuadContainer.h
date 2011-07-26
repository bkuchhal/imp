/**
 *  \file QuadContainer.h    \brief A container for Quads.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMP_QUAD_CONTAINER_H
#define IMP_QUAD_CONTAINER_H

#include "kernel_config.h"
#include "internal/IndexingIterator.h"
#include "Particle.h"
#include "container_base.h"
#include "VersionInfo.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "ParticleTuple.h"
#include "QuadScore.h"
#include "QuadModifier.h"
#include "QuadDerivativeModifier.h"
#include "macros.h"

IMP_BEGIN_NAMESPACE

class QuadContainer;
typedef std::pair<QuadContainer*,
                  QuadContainer*> QuadContainerPair;

//! A shared container for Quads
/** Stores a searchable shared collection of Quads.
    \ingroup restraints

    \implementationwithoutexample{QuadContainer, IMP_QUAD_CONTAINER}
 */
class IMPEXPORT QuadContainer : public Container
{
  mutable internal::OwnerPointer<Container> added_, removed_;
  struct Accessor {
    typedef ParticleQuad result_type;
    typedef unsigned int argument_type;
    result_type operator()(argument_type i) const {
      return o_->get_particle_quad(i);
    }
    Accessor(QuadContainer *pc): o_(pc){}
    Accessor(): o_(NULL){}
    IMP_COMPARISONS_1(Accessor, o_);
  private:
    // This should be ref counted, but swig memory management is broken
    QuadContainer* o_;
  };
 protected:
  /** Containers must have containers that keep track of the particles
      which have been added or since the last step. These containers
      must be registered with the parent QuadContainer.

      Containers which are themselves returned by the get_added/removed
      functions do not have to register such containers.
  */
  virtual QuadContainerPair
    get_added_and_removed_containers() const =0;
  bool get_has_added_and_removed_containers() const {
    return (added_ && added_->get_is_shared())
      || (removed_ && removed_->get_is_shared());
  }
  QuadContainer(){}
  QuadContainer(Model *m,
                     std::string name="QuadContainer %1%");
#ifndef IMP_DOXYGEN
  template <class S>
    double call_evaluate_index(const S *s,
                         const ParticleIndexQuad& a,
                         DerivativeAccumulator *da) const {
    return s->S::evaluate_index(get_model(), a, da);
  }
  double call_evaluate_index(const QuadScore *s,
                              const ParticleIndexQuad& a,
                              DerivativeAccumulator *da) const {
    return s->evaluate_index(get_model(), a, da);
  }
  template <class S>
    double call_evaluate_if_good_index(const S *s,
                                 const ParticleIndexQuad& a,
                                 DerivativeAccumulator *da,
                                 double max) const {
    return s->S::evaluate_if_good_index(get_model(), a, da, max);
  }
  double call_evaluate_if_good_index(const QuadScore *s,
                                      const ParticleIndexQuad& a,
                                      DerivativeAccumulator *da,
                                      double max) const {
    return s->evaluate_if_good_index(get_model(), a, da, max);
  }
  template <class S>
    void call_apply_index(const S *s,
                    const ParticleIndexQuad& a) const {
    s->S::apply_index(get_model(), a);
  }
  void call_apply(const QuadModifier *s,
                         const ParticleIndexQuad& a) const {
    s->apply_index(get_model(), a);
  }
  template <class S>
    void call_apply_index(const S *s,
                           const ParticleIndexQuad& a,
                           DerivativeAccumulator *&da) const {
    s->S::apply_index(get_model(), a, da);
  }
  void call_apply_index(const QuadDerivativeModifier *s,
                  const ParticleIndexQuad& a,
                  DerivativeAccumulator &da) const {
    s->apply_index(get_model(), a, da);
  }
#endif
public:
  typedef ParticleQuad ContainedType;
  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle_quad(const ParticleQuad& v) const =0;
  //! return the number of Quads in the container
  /** \note this isn't always constant time
   */
  virtual unsigned int get_number_of_particle_quads() const =0;

  ParticleQuadsTemp get_particle_quads() const {
    return ParticleQuadsTemp(particle_quads_begin(),
                              particle_quads_end());
  }
  virtual ParticleQuad get_particle_quad(unsigned int i) const=0;

#ifdef IMP_DOXYGEN
  //! An iterator through the contents of the container
  class ParticleQuadIterator;
#else
  typedef internal::IndexingIterator<Accessor> ParticleQuadIterator;
#endif
#ifndef SWIG
  //! begin iterating through the Quads
  ParticleQuadIterator particle_quads_begin() const {
    // Since I can't make the count mutable in Object
    return
      ParticleQuadIterator(Accessor(const_cast<QuadContainer*>(this)),
                        0);
  }
  //! iterate through the Quads
  ParticleQuadIterator particle_quads_end() const {
    return
      ParticleQuadIterator(Accessor(const_cast<QuadContainer*>(this)),
                        get_number_of_particle_quads());
    }
#endif

  //! Apply a SingletonModifier to the contents
  virtual void apply(const QuadModifier *sm)=0;

  //! Apply a SingletonModifier to the contents
  virtual void apply(const QuadDerivativeModifier *sm,
                     DerivativeAccumulator &da)=0;

  //! Evaluate a score on the contents
  virtual double evaluate(const QuadScore *s,
                          DerivativeAccumulator *da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate_if_good(const QuadScore *s,
                                  DerivativeAccumulator *da,
                                  double max) const=0;


  /** \name Tracking changes
      The container can keep track of the changes since the last
      Model::evaluate() call. To use this, make sure to call
      get_removed_container() or get_added_container() before
      Model::evaluate() so the container knows to track changes.
      @{
  */
  QuadContainer* get_removed_container() const {
    // must not be an added or removed container
    get_model();
    if (!added_) {
      std::pair<QuadContainer*, QuadContainer*>
        cp= get_added_and_removed_containers();
      added_=cp.first;
      removed_=cp.second;
    }
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_container() do not "
                    << " track their own added and removed contents.");
    QuadContainer *ret= dynamic_cast<QuadContainer*>(removed_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << removed_->get_name()
                       << " to a QuadContainer.");
    return ret;
  }
  QuadContainer* get_added_container() const {
    // must not be an added or removed container
    if (!added_) {
      std::pair<QuadContainer*, QuadContainer*>
        cp= get_added_and_removed_containers();
      added_=cp.first;
      removed_=cp.second;
    }
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_container() do not "
                    << " track their own added and removed contents.");
    QuadContainer *ret= dynamic_cast<QuadContainer*>(added_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << added_->get_name()
                       << " to a QuadContainer.");
    return ret;
  }
  /** @} */

#ifndef IMP_DOXYGEN
  typedef ParticleQuad value_type;
  ParticleQuad get(unsigned int i) const {return get_particle_quad(i);}
  ParticleQuadsTemp get() const {
    return get_particle_quads();
  }
  bool get_contains(const ParticleQuad& v) const {
    return get_contains_particle_quad(v);
  }
  unsigned int get_number() const {return get_number_of_particle_quads();}
#ifndef SWIG
  virtual bool get_provides_access() const {return false;}
  virtual const ParticleIndexQuads& get_access() const {
    IMP_THROW("Object not implemented properly.", IndexException);
  }
#endif
  virtual ParticleIndexQuads get_indexes() const {
    ParticleIndexQuads ret(get_number());
    for (unsigned int i=0; i< ret.size(); ++i) {
      ret[i]= IMP::internal::get_index(get(i));
    }
    return ret;
  }
#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(QuadContainer);
};

IMP_OBJECTS(QuadContainer,QuadContainers);

IMP_END_NAMESPACE

#endif  /* IMP_QUAD_CONTAINER_H */
