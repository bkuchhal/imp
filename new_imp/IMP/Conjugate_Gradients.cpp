/*
 *  Conjugate_Gradients.cpp
 *  IMP
 *
 *  Copyright 2007 Sali Lab. All rights reserved.
 *
 */

#include <cmath>

#include "Conjugate_Gradients.h"
#include "log.h"

namespace imp
{

/** Estimate of limit of machine precision */
static const float eps = 1.2e-7;

/** Get the score for the model with the current state of optimizable variables
    x, and return the derivatives in g. */
static Float get_score(Model &model, Model_Data *model_data,
                       std::vector<Float_Index> float_indices,
                       std::vector<Float> &x, std::vector<Float> &dscore)
{
  int i, opt_var_cnt = float_indices.size();
  /* set model state */
  for (i = 0; i < opt_var_cnt; i++) {
    model_data->set_float(float_indices[i], x[i]);
  }

  /* get score */
  Float score = model.evaluate(true);

  /* get derivatives */
  for (i = 0; i < opt_var_cnt; i++) {
    dscore[i] = model_data->get_deriv(float_indices[i]);
  }
  return score;
}

/**
  Constructor
 */
Conjugate_Gradients::Conjugate_Gradients()
{
}


/**
  Destructor
 */
Conjugate_Gradients::~Conjugate_Gradients()
{
}


/**
 * Conjugate gradients optimization, as per Shanno and Phua, ACM Transactions
 * On Mathematical Software 6 (December 1980), 618-622
 *
 * \param[in] model     Model that is being optimized.
 * \param[in] max_steps Maximum number of iterations before aborting.
 * \param[in] threshold Largest acceptable gradient-squared value
 *                      for convergence.
 *
 * \return score of the final state of the model.
 */
Float Conjugate_Gradients::optimize(Model& model, int max_steps,
                                    Float threshold)
{
  std::vector<Float_Index> float_indices;
  std::vector<Float> x, dx;
  int n = 0, i;
  Model_Data* model_data = model.get_model_data();

  Opt_Float_Index_Iterator opt_float_iter;

  opt_float_iter.reset(model_data);
  // determine n, the number of degrees of freedom
  while (opt_float_iter.next()) {
    n++;
    float_indices.push_back(opt_float_iter.get());
  }

  x.resize(n);
  dx.resize(n);
  // get initial state in x(n):
  for (i = 0; i < n; i++) {
    x[i] = model_data->get_float(float_indices[i]);
  }

  // INITIALIZE ITER,IFUN,NFLAG,AND IOUTK,WHICH COUNTS OUTPUT ITERATIONS.
  int iter = 0, ifun = 0, nrst, ncalls, nflag = 0;
  float dg1, xsq, gsq, f = 0., alpha, dg = 1., minf, ap, fp, dp, step, dal, u1,
        u2, u3, u4, at, w1 = 0., w2 = 0., rtst, bestf;
  bool rsw;

  // dx holds the gradient at x
  // search holds the search vector
  // estimate holds the best current estimate to the minimizer
  // destimate holds the gradient at the best current estimate
  // resy holds the restart Y vector
  // ressearch holds the restart search vector
  std::vector<Float> search, estimate, destimate, resy, ressearch;
  search.resize(n);
  estimate.resize(n);
  destimate.resize(n);
  resy.resize(n);
  ressearch.resize(n);

/* CALCULATE THE FUNCTION AND GRADIENT AT THE INITIAL
  POINT AND INITIALIZE NRST,WHICH IS USED TO DETERMINE
  WHETHER A BEALE RESTART IS BEING DONE. NRST=N MEANS THAT THIS
  ITERATION IS A RESTART ITERATION. INITIALIZE RSW,WHICH INDICATES
  THAT THE CURRENT SEARCH DIRECTION IS A GRADIENT DIRECTION. */
g20: f = get_score(model, model_data, float_indices, x, dx);
    ifun++;
    nrst = n;
    rsw = true;

/* CALCULATE THE INITIAL SEARCH DIRECTION , THE NORM OF X SQUARED,
  AND THE NORM OF G SQUARED. DG1 IS THE CURRENT DIRECTIONAL
  DERIVATIVE,WHILE XSQ AND GSQ ARE THE SQUARED NORMS. */
  dg1 = xsq = 0.;

  for (i = 0; i < n; i++) {
    search[i] = -dx[i];
    xsq += x[i] * x[i];
    dg1 -= dx[i] * dx[i];
  }
  gsq = -dg1;

/* TEST IF THE INITIAL POINT IS THE MINIMIZER. */
  if (gsq <= eps * eps * std::max(1.0f, xsq)) {
    goto end;
  }

/* BEGIN THE MAJOR ITERATION LOOP. NCALLS IS USED TO GUARANTEE THAT
  AT LEAST TWO POINTS HAVE BEEN TRIED. minf IS THE
  CURRENT FUNCTION VALUE. */
g40: minf = f;
  ncalls = ifun;

/* BEGIN LINEAR SEARCH. ALPHA IS THE STEPLENGTH. */

  if (rsw) {
    /* IF A GRADIENT DIRECTION IS USED, SET ALPHA=1.0/DSQRT(GSQ),
       WHICH SCALES THE INITIAL SEARCH VECTOR TO UNITY. */
    alpha = 1.0 / sqrt(gsq);
  } else if (nrst == 1) {
    /* IF A RESTART HAS BEEN PERFORMED, SET ALPHA=1.0. */
    alpha = 1.0;
  } else {
    /* SET ALPHA TO THE NONRESTART CONJUGATE GRADIENT ALPHA. */
    alpha = alpha * dg / dg1;
  }


/* THE LINEAR SEARCH FITS A CUBIC TO F AND DAL, THE FUNCTION AND ITS
  DERIVATIVE AT ALPHA, AND TO FP AND DP,THE FUNCTION
  AND DERIVATIVE AT THE PREVIOUS TRIAL POINT AP.
  INITIALIZE AP ,FP,AND DP. */
  ap = 0.;
  fp = minf;
  dp = dg1;

/* SAVE THE CURRENT DERIVATIVE TO SCALE THE NEXT SEARCH VECTOR. */
  dg = dg1;

/* UPDATE THE ITERATION. */
  iter++;

/* CALCULATE THE CURRENT STEPLENGTH  AND STORE THE CURRENT X AND G. */
  step = 0.;
  for (i = 0; i < n; i++) {
    step += search[i] * search[i];
    estimate[i] = x[i];
    destimate[i] = dx[i];
  }
  step = sqrt(step);

/* BEGIN THE LINEAR SEARCH ITERATION.
  TEST FOR FAILURE OF THE LINEAR SEARCH. */

g80: if (alpha * step <= eps) {

  /* TEST IF DIRECTION IS A GRADIENT DIRECTION. */
    if (!rsw) {
      goto g20;
    } else {
      nflag = 2;
      goto end;
    }
  }

/* CALCULATE THE TRIAL POINT. */
  for (i = 0; i < n; i++) {
    x[i] = estimate[i] + alpha * search[i];
  }

/* EVALUATE THE FUNCTION AT THE TRIAL POINT. */
  f = get_score(model, model_data, float_indices, x, dx);

/* TEST IF THE MAXIMUM NUMBER OF FUNCTION CALLS HAVE BEEN USED. */
  ifun++;
  if (ifun > max_steps) {
    nflag = 1;
    goto end;
  }

/* COMPUTE THE DERIVATIVE OF F AT ALPHA. */
  dal = 0.0;
  for (i = 0; i < n; i++) {
    dal += dx[i] * search[i];
  }

/* TEST WHETHER THE NEW POINT HAS A NEGATIVE SLOPE BUT A HIGHER
  FUNCTION VALUE THAN ALPHA=0. IF THIS IS THE CASE,THE SEARCH
  HAS PASSED THROUGH A LOCAL MAX AND IS HEADING FOR A DISTANT LOCAL
  MINIMUM. */
  if (f > minf && dal < 0.) {
    goto g160;
  }

/* IF NOT, TEST WHETHER THE STEPLENGTH CRITERIA HAVE BEEN MET. */
  if (f <= (minf + 0.0001 * alpha * dg) && fabs(dal / dg) <= 0.9) {

/* IF THEY HAVE BEEN MET, TEST IF TWO POINTS HAVE BEEN TRIED
   AND IF THE TRUE LINE MINIMUM HAS NOT BEEN FOUND. */
    if (ifun - ncalls > 1 || fabs(dal / dg) <= eps) {
      goto g170;
    }
  }

/* A NEW POINT MUST BE TRIED. USE CUBIC INTERPOLATION TO FIND
  THE TRIAL POINT AT. */

  u1 = dp + dal - 3.0 * (fp - f) / (ap - alpha);
  u2 = u1 * u1 - dp * dal;
  if (u2 < 0.) {
    u2 = 0.;
  }
  u2 = sqrt(u2);
  at = alpha - (alpha - ap) * (dal + u2 - u1) / (dal - dp + 2. * u2);

/* TEST WHETHER THE LINE MINIMUM HAS BEEN BRACKETED. */
  if (dal / dp <= 0.) {

/* THE MINIMUM HAS BEEN BRACKETED. TEST WHETHER THE TRIAL POINT LIES
  SUFFICIENTLY WITHIN THE BRACKETED INTERVAL.
  IF IT DOES NOT, CHOOSE AT AS THE MIDPOINT OF THE INTERVAL. */
    if (at < (1.01 * std::min(alpha, ap))
        || at > (0.99 * std::max(alpha, ap))) {
      at = (alpha + ap) / 2.0;
    }

/* THE MINIMUM HAS NOT BEEN BRACKETED. TEST IF BOTH POINTS ARE
  GREATER THAN THE MINIMUM AND THE TRIAL POINT IS SUFFICIENTLY
  SMALLER THAN EITHER. */
  } else if (dal <= 0.0 || 0.0 >= at || at >= (0.99 * std::min(ap, alpha))) {

/* TEST IF BOTH POINTS ARE LESS THAN THE MINIMUM AND THE TRIAL POINT
  IS SUFFICIENTLY LARGE. */
    if (dal > 0.0 || at <= (1.01 * std::max(ap, alpha))) {

/* IF THE TRIAL POINT IS TOO SMALL,DOUBLE THE LARGEST PRIOR POINT. */
      if (dal <= 0.0) {
        at = 2.0 * std::max(ap, alpha);
      }

/* IF THE TRIAL POINT IS TOO LARGE, HALVE THE SMALLEST PRIOR POINT. */
      if (dal > 0.) {
        at = std::min(ap, alpha) / 2.0;
      }
    }
  }

/* SET AP=ALPHA, ALPHA=AT,AND CONTINUE SEARCH. */
  ap = alpha;
  fp = f;
  dp = dal;
  alpha = at;
  goto g80;

/* A RELATIVE MAX HAS BEEN PASSED.REDUCE ALPHA AND RESTART THE SEARCH. */
g160: alpha /= 3.0;
  ap = 0.;
  fp = minf;
  dp = dg;
  goto g80;

/* THE LINE SEARCH HAS CONVERGED. TEST FOR CONVERGENCE OF THE ALGORITHM. */
g170: gsq = xsq = 0.0;
  for (i = 0; i < n; i++) {
    gsq += dx[i] * dx[i];
    xsq += x[i] * x[i];
  }
  if (gsq < threshold) {
    goto end;
  }

/* SEARCH CONTINUES. SET W(I)=ALPHA*W(I),THE FULL STEP VECTOR. */
  for (i = 0; i < n; i++) {
    search[i] *= alpha;
  }

/* COMPUTE THE NEW SEARCH VECTOR;
  TEST IF A POWELL RESTART IS INDICATED. */
  rtst = 0.;
  for (i = 0; i < n; i++) {
    rtst += dx[i] * destimate[i];
  }
  if (fabs(rtst / gsq) > 0.2) {
    nrst = n;
  }

/* IF A RESTART IS INDICATED, SAVE THE CURRENT D AND Y
  AS THE BEALE RESTART VECTORS AND SAVE D'Y AND Y'Y
  IN W(NCONS+1) AND W(NCONS+2). */
  if (nrst == n) {
    w1 = w2 = 0.;
    for (i = 0; i < n; i++) {
      resy[i] = dx[i] - destimate[i];
      ressearch[i] = search[i];
      w1 += resy[i] * resy[i];
      w2 += search[i] * resy[i];
    }
  }

/* CALCULATE  THE RESTART HESSIAN TIMES THE CURRENT GRADIENT. */
  u1 = u2 = 0.0;
  for (i = 0; i < n; i++) {
    u1 -= ressearch[i] * dx[i] / w1;
    u2 += ressearch[i] * dx[i] * 2.0 / w2 - resy[i] * dx[i] / w1;
  }
  u3 = w2 / w1;
  for (i = 0; i < n; i++) {
    estimate[i] = -u3 * dx[i] - u1 * resy[i] - u2 * ressearch[i];
  }

/* IF THIS IS A RESTART ITERATION,W(NX+I) CONTAINS THE NEW SEARCH
  VECTOR. */
  if (nrst != n) {

/* NOT A RESTART ITERATION. CALCULATE THE RESTART HESSIAN
  TIMES THE CURRENT Y. */
    u1 = u2 = u3 = 0.0;
    for (i = 0; i < n; i++) {
      u1 -= (dx[i] - destimate[i]) * ressearch[i] / w1;
      u2 = u2 - (dx[i] - destimate[i]) * resy[i] / w1
           + 2.0 * ressearch[i] * (dx[i] - destimate[i]) / w2;
      u3 += search[i] * (dx[i] - destimate[i]);
    }
    step = u4 = 0.;
    for (i = 0; i < n; i++) {
      step = (w2 / w1) * (dx[i] - destimate[i])
             + u1 * resy[i] + u2 * ressearch[i];
      u4 += step * (dx[i] - destimate[i]);
      destimate[i] = step;
    }

/* CALCULATE THE DOUBLY UPDATED HESSIAN TIMES THE CURRENT
  GRADIENT TO OBTAIN THE SEARCH VECTOR. */
    u1 = u2 = 0.0;
    for (i = 0; i < n; i++) {
      u1 -= search[i] * dx[i] / u3;
      u2 += (1.0 + u4 / u3) * search[i] * dx[i] / u3
            - destimate[i] * dx[i] / u3;
    }
    for (i = 0; i < n; i++) {
      estimate[i] = estimate[i] - u1 * destimate[i] - u2 * search[i];
    }
  }

/* CALCULATE THE DERIVATIVE ALONG THE NEW SEARCH VECTOR. */
  dg1 = 0.0;
  for (i = 0; i < n; i++) {
    search[i] = estimate[i];
    dg1 += search[i] * dx[i];
  }

/* IF THE NEW DIRECTION IS NOT A DESCENT DIRECTION,STOP. */
  if (dg1 <= 0.0) {

/* UPDATE NRST TO ASSURE AT LEAST ONE RESTART EVERY N ITERATIONS. */
    if (nrst == n) {
      nrst = 0;
    }
    nrst++;
    rsw = false;
    goto g40;
  }

/* ROUNDOFF HAS PRODUCED A BAD DIRECTION. */
  nflag = 3;

end:
  // If the 'best current estimate' is better than the current state, return
  // that:
  bestf = get_score(model, model_data, float_indices, estimate, destimate);
  if (bestf < f) {
    for (i = 0; i < n; i++) {
      x[i] = estimate[i];
    }
    f = bestf;
  }
  // Set final model state
  for (i = 0; i < n; i++) {
    model_data->set_float(float_indices[i], x[i]);
  }
  return f;
}

}  // namespace imp
