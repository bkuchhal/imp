

#ifdef IMP_USE_CGAL
%pythoncode %{
has_cgal=True
%}
#else
%pythoncode %{
has_cgal=False
%}
#endif

#ifdef IMP_USE_MODELLER
%pythoncode %{
have_modeller=True
%}
#else
%pythoncode %{
have_modeller=False
%}
#endif

// Make sure that Python refcounts any returned pointers to IMP types
IMP_REFCOUNT_RETURN_SINGLE(IMP::Particle)
IMP_REFCOUNT_RETURN_SINGLE(IMP::Restraint)
IMP_REFCOUNT_RETURN_SINGLE(IMP::ScoreState)
IMP_REFCOUNT_RETURN_SINGLE(IMP::OptimizerState)


%ignore IMP::ParticlePair::operator[];
%ignore IMP::ParticleTriplet::operator[];
%extend IMP::ParticlePair {
  Particle* __getitem__(unsigned int index) const {
    return self->operator[](index);
  }
  void __setitem__(unsigned int index, Particle* val) {
    self->operator[](index) = val;
  }
  int __len__() const {
      return 2;
  }
}
%extend IMP::ParticleTriplet {
  Particle* __getitem__(unsigned int index) const {
    return self->operator[](index);
  }
  void __setitem__(unsigned int index, Particle* val) {
    self->operator[](index) = val;
  }
  int __len__() const {
      return 3;
  }
}  

namespace IMP {
  %typemap(out) std::pair<Float,Float> {
     PyObject *tup= PyTuple_New(2);
     PyTuple_SetItem(tup, 0, PyFloat_FromDouble($1.first));
     PyTuple_SetItem(tup, 1, PyFloat_FromDouble($1.second));
     $result= tup;
  }
}


%extend IMP::Object {
   std::string __str__() const {
     std::ostringstream oss;
     self->show(oss);
     return oss.str();
   }
}

namespace IMP {
  // special case since particles are ref counted
  %extend Model {
    Particles get_particles() const {
      IMP::Particles ret(self->particles_begin(), self->particles_end());
      return ret;
    }
  }
  IMP_CONTAINER_SWIG(Model, ScoreState, score_state)
  IMP_CONTAINER_SWIG(Model, Restraint, restraint)
  IMP_CONTAINER_SWIG(Optimizer, OptimizerState, optimizer_state)
}

%feature("ref")   IMP::RefCounted "IMP::internal::ref($this);"
%feature("unref") IMP::RefCounted "IMP::internal::unref($this);"

/* Make selected classes extensible in Python */
IMP_DIRECTOR_KERNEL_CLASS(UnaryFunction);
IMP_DIRECTOR_KERNEL_CLASS(Restraint);
IMP_DIRECTOR_KERNEL_CLASS(ScoreState);
IMP_DIRECTOR_KERNEL_CLASS(OptimizerState);
IMP_DIRECTOR_KERNEL_CLASS(SingletonScore);
IMP_DIRECTOR_KERNEL_CLASS(PairScore);
IMP_DIRECTOR_KERNEL_CLASS(TripletScore);
IMP_DIRECTOR_KERNEL_CLASS(Optimizer);
IMP_DIRECTOR_KERNEL_CLASS(Refiner);
IMP_DIRECTOR_KERNEL_CLASS(SingletonContainer);
IMP_DIRECTOR_KERNEL_CLASS(PairContainer);
IMP_DIRECTOR_KERNEL_CLASS(SingletonModifier);
IMP_DIRECTOR_KERNEL_CLASS(PairModifier);

%include "IMP/utility.h"
%include "IMP/Key.h"

namespace IMP {
  %template(FloatKeyBase) ::IMP::KeyBase<0, true>;
  %template(IntKeyBase) ::IMP::KeyBase<1, true>;
  %template(StringKeyBase) ::IMP::KeyBase<2, true>;
  %template(ParticleKeyBase) ::IMP::KeyBase<3, true>;
  %template(ObjectKeyBase) ::IMP::KeyBase<4, true>;
}

%include "IMP/log.h"
%include "IMP/base_types.h"

%include "IMP/utility.h"
%include "IMP/directories.h"
%include "IMP/deprecation.h"
%include "IMP/VersionInfo.h"
%include "IMP/RefCounted.h"
%include "IMP/VectorOfRefCounted.h"
%include "Object.i"
%include "IMP/FailureHandler.h"
%include "IMP/DerivativeAccumulator.h"
%include "Particle.i"
%include "IMP/Decorator.h"


%include "IMP/UnaryFunction.h"
%include "Restraint.i"
%include "IMP/ScoreState.h"
%include "IMP/OptimizerState.h"
%include "IMP/Model.h"
%include "IMP/PairScore.h"
%include "IMP/Refiner.h"
%include "IMP/SingletonScore.h"
%include "IMP/TripletScore.h"
%include "random.i"
%include "IMP/Optimizer.h"
%include "IMP/SingletonModifier.h"
%include "IMP/PairModifier.h"
%include "IMP/SingletonContainer.h"
%include "IMP/PairContainer.h"


namespace IMP {
  %template(Particles) ::IMP::VectorOfRefCounted<Particle*> ;
  %template(ParticlesTemp) ::std::vector<Particle*>;
  %template(ParticlesList) ::std::vector<ParticlesTemp>;
  %template(ParticlesPairsTemp) ::std::vector<ParticlePair>;
  %template(ParticlePairs) ::IMP::VectorOfRefCounted< ParticlePair, RefCountParticlePair>;
  %template(ParticleTriplets) ::IMP::VectorOfRefCounted< ParticleTriplet, RefCountParticleTriplet>;
  %template(Restraints) ::IMP::VectorOfRefCounted< Restraint*>;
  %template(ScoreStates) ::IMP::VectorOfRefCounted< ScoreState*>;
  %template(OptimizerStates) ::IMP::VectorOfRefCounted< OptimizerState*>;
  %template(Refiners) ::IMP::VectorOfRefCounted< Refiner*>;
  %template(FloatKeys) ::std::vector<FloatKey>;
  %template(StringKeys) ::std::vector<StringKey>;
  %template(IntKeys) ::std::vector<IntKey>;
  %template(ParticleKeys) ::std::vector<ParticleKey>;
  %template(ObjectKeys) ::std::vector<ObjectKey>;
  %template(Floats) ::std::vector<Float>;
  %template(Strings) ::std::vector<String>;
  %template(Ints) ::std::vector<Int>;
  %template(SingletonContainers) ::IMP::VectorOfRefCounted<SingletonContainer*>;
  %template(PairContainers) ::IMP::VectorOfRefCounted<PairContainer*>;
  %template(float_pair) ::std::pair<float,float>;
  %template(double_pair) ::std::pair<double, double>;
}
