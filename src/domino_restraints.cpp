/**
 *  \file domino_restraints.cpp
 *  \brief Membrane domino stuff
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/core.h>
#include <IMP/algebra.h>
#include <IMP/atom.h>
#include <IMP/container.h>
#include <IMP/membrane.h>

using namespace IMP;
using namespace IMP::membrane;

IMPMEMBRANE_BEGIN_NAMESPACE

RestraintSet* create_restraints
(Model *m,atom::Hierarchy protein,core::TableRefiner *tbr,Parameters* myparam)
{
HelixData *TM=&(myparam->TM);
RstParameters *RST=&(myparam->RST);

IMP_NEW(RestraintSet,rset,());
FloatRange z_range = FloatRange(-RST->zeta,RST->zeta);
FloatRange tilt_range = FloatRange(0.0,RST->tilt);
double kappa=RST->kappa;
double kappa2=RST->kappa2;

// single-body restraints
for(int i=0;i<TM->num;++i){
 atom::Selection s=atom::Selection(protein);
 s.set_molecule(TM->name[i]);
 core::RigidBody rb=
 core::RigidMember(s.get_selected_particles()[0]).get_rigid_body();
 add_depth_restraint(m,rb,z_range,kappa2);
 add_tilt_restraint(m,rb,tilt_range,kappa2);
}

// multi-body restraints
add_excluded_volume(m,protein,kappa);
if(RST->add_dope) add_DOPE(m,protein,RST->score_name);
if(RST->add_pack) add_packing_restraint(m,protein,tbr,TM,kappa);
add_diameter_restraint(m,protein,RST->diameter,TM,kappa);

// two-body restraints
for(unsigned int i=0;i<TM->loop.size();++i){
 int i0=TM->loop[i].first;
 int i1=TM->loop[i].second;
 atom::Selection s0=atom::Selection(protein);
 s0.set_atom_type(atom::AT_CA);
 s0.set_residue_index(TM->resid[i0].second);
 atom::Selection s1=atom::Selection(protein);
 s1.set_atom_type(atom::AT_CA);
 s1.set_residue_index(TM->resid[i1].first);
 Particle *p0=s0.get_selected_particles()[0];
 Particle *p1=s1.get_selected_particles()[0];
// End-to-End distance restraint
 double length=1.6*(double(TM->resid[i1].first-TM->resid[i0].second+1))+7.4;
 if(RST->add_endtoend)
  core::PairRestraint* lr=add_distance_restraint(m,p0,p1,length,kappa);
// COM-COM distance restraint
 core::RigidBody rb0=core::RigidMember(p0).get_rigid_body();
 core::RigidBody rb1=core::RigidMember(p1).get_rigid_body();
 core::PairRestraint* lrb=
 add_distance_restraint(m,rb0,rb1,RST->cm_dist,kappa);
 rset->add_restraint(lrb);
}
for(unsigned int i=0;i<TM->inter.size();++i){
 int i0=TM->inter[i].first;
 int i1=TM->inter[i].second;
 atom::Selection s0=atom::Selection(protein);
 s0.set_molecule(TM->name[i0]);
 atom::Selection s1=atom::Selection(protein);
 s1.set_molecule(TM->name[i1]);
 core::RigidBody rb0=
 core::RigidMember(s0.get_selected_particles()[0]).get_rigid_body();
 core::RigidBody rb1=
 core::RigidMember(s1.get_selected_particles()[0]).get_rigid_body();
 core::PairRestraint* ir=add_interacting_restraint(m,rb0,rb1,tbr,
                         RST->d0_inter,kappa);
 rset->add_restraint(ir);
}
return rset.release();
}

void add_excluded_volume
(Model *m,atom::Hierarchy protein,double kappa_)
{
IMP_NEW(container::ListSingletonContainer,lsc,(m));
atom::Selection s=atom::Selection(protein);
s.set_atom_type(atom::AT_CA);
lsc->add_particles(s.get_selected_particles());
IMP_NEW(core::ExcludedVolumeRestraint,evr,(lsc,kappa_));
evr->set_name("Excluded Volume");
m->add_restraint(evr);
m->set_maximum_score(evr, max_score_);
}

void add_DOPE(Model *m,atom::Hierarchy protein,std::string sname)
{
atom::add_dope_score_data(protein);
IMP_NEW(container::ListSingletonContainer,lsc,(m));
atom::Selection s=atom::Selection(protein);
s.set_atom_type(atom::AT_CA);
lsc->add_particles(s.get_selected_particles());
IMP_NEW(container::ClosePairContainer,cpc,(lsc, 15.0));
IMP_NEW(SameHelixPairFilter,f,());
cpc->add_pair_filter(f);
IMP_NEW(atom::DopePairScore,dps,(15.0,atom::get_data_path(sname)));
IMP_NEW(container::PairsRestraint,dope,(dps,cpc));
dope->set_name("DOPE scoring function");
m->add_restraint(dope);
m->set_maximum_score(dope, max_score_);
}

void add_packing_restraint
(Model *m,atom::Hierarchy protein,core::TableRefiner *tbr,
HelixData *TM,double kappa_)
{
//if the helices are interacting, apply a filter on the crossing angle
//first define the allowed intervals, by specifying the center
//of the distributions (Walters and DeGrado PNAS (2007) 103:13658)
const double om0[]={-156.5, 146.4, -37.9, 13.8, 178.0, 25.5, -161.1, 44.8,
127.4, -60.2, -129.2, 2.4, 161.0};
//the sigmas
const double sig_om0[]={10.1, 13.6, 7.50, 16.6, 20.8, 11.2, 10.3, 8.8, 12.3,
14.8, 12.9, 16.2, 17.6};
//  distance cutoff
const double dd0[]={8.61, 8.57, 7.93, 9.77, 9.14, 8.55, 9.30, 7.96, 9.40, 8.61,
8.97, 8.55, 8.75};
//  and distance sigmas
const double sig_dd0[]={0.89, 0.99, 0.88, 1.18, 1.47, 1.05, 1.57, 1.13, 1.0,
1.04, 1.65, 0.78, 1.33};
//  the allowed number of sigma
int nsig=packing_nsig_;
//  and the number of clusters
int ncl=packing_ncl_;
// create allowed intervals (omega in radians)
std::vector<double> om_b, om_e, dd_b, dd_e;

for(int i=0;i<ncl;++i){
 dd_b.push_back(dd0[i]-double(nsig)*sig_dd0[i]);
 dd_e.push_back(dd0[i]+double(nsig)*sig_dd0[i]);
 om_b.push_back(std::max(radians(om0[i]-double(nsig)*sig_om0[i]),-IMP::PI));
 om_e.push_back(std::min(radians(om0[i]+double(nsig)*sig_om0[i]), IMP::PI));
}
IMP_NEW(container::ListSingletonContainer,lrb,(m));
for(int i=0;i<TM->num;++i){
 atom::Selection s=atom::Selection(protein);
 s.set_molecule(TM->name[i]);
 core::RigidBody rb
 =core::RigidMember(s.get_selected_particles()[0]).get_rigid_body();
 lrb->add_particle(rb);
}
IMP_NEW(container::ClosePairContainer,nrb,(lrb, 25.0));
IMP_NEW(RigidBodyPackingScore,ps,(tbr,om_b,om_e,dd_b,dd_e,kappa_));
IMP_NEW(container::PairsRestraint,prs,(ps,nrb));
prs->set_name("Packing restraint");
m->add_restraint(prs);
m->set_maximum_score(prs,max_score_);
}

core::PairRestraint* add_distance_restraint
 (Model *m,Particle *p0,Particle *p1,double x0,double kappa_)
{
IMP_NEW(core::HarmonicUpperBound,hub,(x0,kappa_));
IMP_NEW(core::DistancePairScore,df,(hub));
IMP_NEW(core::PairRestraint,dr,(df, ParticlePair(p0, p1)));
dr->set_name("Distance restraint");
m->add_restraint(dr);
m->set_maximum_score(dr, max_score_);
return dr.release();
}

void add_depth_restraint
(Model *m, Particle *p,FloatRange z_range_,double kappa_)
{
IMP_NEW(core::HarmonicWell,well,(z_range_,kappa_));
IMP_NEW(core::AttributeSingletonScore,ass,(well,FloatKey("z")));
IMP_NEW(core::SingletonRestraint, sr, (ass, p));
sr->set_name("Depth restraint");
m->add_restraint(sr);
m->set_maximum_score(sr, max_score_);
}

void add_tilt_restraint
(Model *m,Particle *p,FloatRange tilt_range_,double kappa_)
{
algebra::Vector3D laxis=algebra::Vector3D(1.0,0.0,0.0);
algebra::Vector3D zaxis=algebra::Vector3D(0.0,0.0,1.0);
IMP_NEW(core::HarmonicWell,well,(tilt_range_, kappa_));
IMP_NEW(TiltSingletonScore,tss,(well, laxis, zaxis));
IMP_NEW(core::SingletonRestraint,sr,(tss, p));
m->add_restraint(sr);
sr->set_name("Tilt restraint");
m->set_maximum_score(sr, max_score_);
}

core::PairRestraint* add_interacting_restraint
(Model *m,Particle *rb0,Particle *rb1,core::TableRefiner *tbr,
double d0_inter_,double kappa_)
{
IMP_NEW(core::HarmonicUpperBound,hub,(d0_inter_, kappa_));
IMP_NEW(core::DistancePairScore,sd,(hub));
IMP_NEW(core::KClosePairsPairScore,kc,(sd,tbr,1));
IMP_NEW(core::PairRestraint,ir,(kc,ParticlePair(rb0,rb1)));
ir->set_name("Interacting restraint");
m->add_restraint(ir);
m->set_maximum_score(ir, max_score_);
return ir.release();
}

void add_diameter_restraint
(Model *m,atom::Hierarchy protein,double diameter_,
HelixData *TM,double kappa_)
{
IMP_NEW(container::ListSingletonContainer,lrb,(m));
for(int i=0;i<TM->num;++i){
 atom::Selection s=atom::Selection(protein);
 s.set_molecule(TM->name[i]);
 core::RigidBody rb
 =core::RigidMember(s.get_selected_particles()[0]).get_rigid_body();
 lrb->add_particle(rb);
}
IMP_NEW(core::HarmonicUpperBound,hub,(0.0,kappa_));
IMP_NEW(core::DiameterRestraint,dr,(hub, lrb, diameter_));
dr->set_name("Diameter restraint");
m->add_restraint(dr);
m->set_maximum_score(dr, max_score_);
}

IMPMEMBRANE_END_NAMESPACE
