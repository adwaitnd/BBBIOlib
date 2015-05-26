/*
 * linspace.c
 *
 * Code generation for function 'linspace'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "linspace.h"

/* Function Definitions */
void linspace(double y[131073])
{
  int k;
  y[131072] = 1.0;
  y[0] = 0.0;
  for (k = 0; k < 131071; k++) {
    y[1 + k] = (1.0 + (double)k) * 7.62939453125E-6;
  }
}

/* End of code generation (linspace.c) */
