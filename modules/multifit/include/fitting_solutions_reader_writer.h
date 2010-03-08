/**
 *  \file fitting_solutions_reader_writer.h
 *  \brief handles reading and writing MultiFit fitting solutions file
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMULTIFIT_FITTING_SOLUTIONS_READER_WRITER_H
#define IMPMULTIFIT_FITTING_SOLUTIONS_READER_WRITER_H

#include <IMP/algebra/Transformation3D.h>
#include <IMP/Model.h>
#include <IMP/core/Hierarchy.h>
#include <IMP/Object.h>
#include <IMP/file.h>
#include "multifit_config.h"
IMPMULTIFIT_BEGIN_NAMESPACE
//! A fitting solution record
/**
\todo discuss with Daniel - should it be an object ?
 */
class IMPMULTIFITEXPORT FittingSolutionRecord {
 public:
  //! Constructor
  /**
  \todo consider using better initialization
  */
  FittingSolutionRecord();
  inline unsigned int get_index() const {return index_;}
  void set_index(unsigned int new_ind) {index_ = new_ind;}
  inline std::string get_solution_filename() const {return sol_fn_;}
  void set_solution_filename(std::string sol_fn) {sol_fn_=sol_fn;}
  inline algebra::Transformation3D get_transformation() const
  {return transformation_;}
  void set_transformation(algebra::Transformation3D t)
   {transformation_=t;}
  inline unsigned int get_match_size() const {return match_size_;}
  void set_match_size(unsigned int match_size)  {match_size_=match_size;}
  inline Float get_match_average_distance() const {return match_avg_dist_;}
  void set_match_average_distance(Float match_avg_dist) {
   match_avg_dist_=match_avg_dist;}
  inline Float get_fitting_score() const {return fitting_score_;}
  void set_fitting_score(Float fit_score) {
    fitting_score_=fit_score;}
  inline Float get_rmsd_to_reference() const{return rmsd_to_ref_;}
  void set_rmsd_to_reference(Float rmsd_to_ref) {
   rmsd_to_ref_=rmsd_to_ref;}
  //! Show
  /**
  \todo consider using initialization mechanism
  */
  void show(std::ostream& out=std::cout) const;
 protected:
  unsigned int index_;
  std::string sol_fn_;
  algebra::Transformation3D transformation_;
  unsigned int match_size_;
  Float match_avg_dist_;
  Float fitting_score_;
  Float rmsd_to_ref_;
};
typedef std::vector<FittingSolutionRecord> FittingSolutionRecords;

//! Fitting solutions reader
/**
\todo consider using TextInput
 */
IMPMULTIFITEXPORT FittingSolutionRecords read_fitting_solutions(
  const char *fitting_fn);

IMPMULTIFITEXPORT void  write_fitting_solutions(
   const char *fitting_fn,
   const FittingSolutionRecords &fit_sols);
IMPMULTIFIT_END_NAMESPACE
#endif /* IMPMULTIFIT_FITTING_SOLUTIONS_READER_WRITER_H */
