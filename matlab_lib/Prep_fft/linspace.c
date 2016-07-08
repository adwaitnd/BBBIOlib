/*
 * linspace.c
 *
 * Code generation for function 'linspace'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep_fft.h"
#include "linspace.h"

/* Function Definitions */
void linspace(double y[32769])
{
  int k;
  y[32768] = 1.0;
  y[0] = 0.0;
  for (k = 0; k < 32767; k++) {
    y[1 + k] = (1.0 + (double)k) * 3.0517578125E-5;
  }
}

/* End of code generation (linspace.c) */
