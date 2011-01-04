/**
 *  \file ListQuadContainer.cpp   \brief A list of ParticleQuadsTemp.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/internal/CoreListQuadContainer.h"
#include "IMP/QuadModifier.h"
#include "IMP/QuadScore.h"
#include <algorithm>


IMPCORE_BEGIN_INTERNAL_NAMESPACE

namespace {
  CoreListQuadContainer* get_list(QuadContainer *g) {
    return dynamic_cast<CoreListQuadContainer*>(g);
  }
}

IMP_ACTIVE_CONTAINER_DEF(CoreListQuadContainer);

CoreListQuadContainer
::CoreListQuadContainer():
  internal::ListLikeQuadContainer(){}


CoreListQuadContainer
::CoreListQuadContainer(Model *m, std::string name):
  internal::ListLikeQuadContainer(m, name){
  initialize_active_container(m);
}


CoreListQuadContainer
::CoreListQuadContainer(Model *m, const char *name):
  internal::ListLikeQuadContainer(m, name){
  initialize_active_container(m);
}


void CoreListQuadContainer::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << get_number_of_particle_quads()
      << " Quads." << std::endl;
}



void CoreListQuadContainer::set_particle_quads(ParticleQuadsTemp sc) {
  update_list(sc);
}


void CoreListQuadContainer::clear_particle_quads() {
  ParticleQuadsTemp t;
  update_list(t);
}


void CoreListQuadContainer::add_particle_quad(const ParticleQuad& vt) {
  IMP_USAGE_CHECK(IMP::internal::is_valid(vt),
                  "Passed Quad cannot be NULL (or None)");
  add_to_list(vt);
  IMP_USAGE_CHECK(!get_has_added_and_removed_containers()
                  || !get_removed_container()
                  ->get_contains(vt),
                  "You cannot remove and add the same item in one time step.");
}

void
CoreListQuadContainer::add_particle_quads(const ParticleQuadsTemp &c) {
  if (c.empty()) return;
  ParticleQuadsTemp cp= c;
  add_to_list(cp);
  IMP_IF_CHECK(USAGE) {
    for (unsigned int i=0; i< c.size(); ++i) {
      IMP_USAGE_CHECK(IMP::internal::is_valid(c[i]),
                    "Passed Quad cannot be NULL (or None)");
      IMP_USAGE_CHECK(!get_has_added_and_removed_containers()
                      || !get_removed_container()
                      ->get_contains(c[i]),
            "You cannot remove and add the same item in one time step.");

    }
  }
}

void CoreListQuadContainer
::remove_particle_quads(const ParticleQuadsTemp &c) {
  if (c.empty()) return;
  ParticleQuadsTemp cp= c;
  remove_from_list(cp);
  IMP_IF_CHECK(USAGE) {
    for (unsigned int i=0; i< c.size(); ++i) {
      IMP_USAGE_CHECK(IMP::internal::is_valid(c[i]),
                    "Passed Quad cannot be NULL (or None)");
    }
  }
}


void CoreListQuadContainer::do_before_evaluate() {
  internal::ListLikeQuadContainer::do_before_evaluate();
}

void CoreListQuadContainer::do_after_evaluate() {
  internal::ListLikeQuadContainer::do_after_evaluate();
}

ParticlesTemp CoreListQuadContainer::get_state_input_particles() const {
  return ParticlesTemp();
}

ContainersTemp CoreListQuadContainer::get_state_input_containers() const {
  return ContainersTemp();
}

IMPCORE_END_INTERNAL_NAMESPACE
