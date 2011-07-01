/**
 *  \file MonteCarloWithWte.cpp
 *  \brief An exotic version of MonteCarlo
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/membrane/MonteCarloWithWte.h>
#include <IMP/core.h>

IMPMEMBRANE_BEGIN_NAMESPACE

MonteCarloWithWte::MonteCarloWithWte(Model *m, double emin,  double emax,
                                               double sigma, double gamma,
                                               double w0):
  core::MonteCarlo(m) {
  min_   = emin;
  max_   = emax;
  sigma_ = sigma;
  gamma_ = gamma;
  w0_    = w0;
  dx_    = sigma / 3.0;
  nbin_  = ceil((emax-emin)/dx_)+1;
  bias_  = new double[nbin_];
  for (unsigned int i=0; i<nbin_; ++i) bias_[i] = 0.0;
  }

double MonteCarloWithWte::get_bias(double score)
{
  int index = floor((score-min_)/dx_);
  if (index < 0)       index=0;
  if (index >= nbin_)  index=nbin_-1;
  return bias_[index];
}

void MonteCarloWithWte::update_bias(double score)
{
  if(score < min_ || score > max_) return;
// first calculate current Gaussian height
  double vbias=get_bias(score);
  double ww = w0_*exp(-vbias/(get_kt()*(gamma_-1)));
// we don't need to run over the entire grid
// let's put a cutoff at 4 sigma
  unsigned int i0=floor((score-4.0*sigma_-min_)/dx_);
  unsigned int i1=floor((score+4.0*sigma_-min_)/dx_);
  i0=max(i0,0);
  i1=min(i1,nbin_);
  for (unsigned int i = i0; i <= i1; ++i){
   double xx = min_ + i * dx_;
   double dp = ( xx - score ) / sigma_;
   bias_[i] += ww * exp ( - 0.5 * dp * dp );
  }
}

IMPMEMBRANE_END_NAMESPACE
