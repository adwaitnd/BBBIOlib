/*
 * bsxfun.c
 *
 * Code generation for function 'bsxfun'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "bsxfun.h"

/* Function Definitions */
void bsxfun(const creal32_T a[262144], const creal32_T b[262144], creal32_T c
            [262144])
{
  int ak;
  int bk;
  int ck;
  ak = 0;
  bk = 0;
  for (ck = 0; ck < 262144; ck++) {
    c[ck].re = a[ak].re * b[bk].re - a[ak].im * b[bk].im;
    c[ck].im = a[ak].re * b[bk].im + a[ak].im * b[bk].re;
    ak++;
    bk++;
  }
}

/* End of code generation (bsxfun.c) */
