/*
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep_fft.h"
#include "abs.h"

/* Function Declarations */
static float rt_hypotf_snf(float u0, float u1);

/* Function Definitions */
static float rt_hypotf_snf(float u0, float u1)
{
  float y;
  float a;
  float b;
  a = (real32_T)fabs(u0);
  b = (real32_T)fabs(u1);
  if (a < b) {
    a /= b;
    y = b * (real32_T)sqrt(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * (real32_T)sqrt(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = b;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

void b_abs(const creal32_T x[32769], float y[32769])
{
  int k;
  for (k = 0; k < 32769; k++) {
    y[k] = rt_hypotf_snf(x[k].re, x[k].im);
  }
}

/* End of code generation (abs.c) */
