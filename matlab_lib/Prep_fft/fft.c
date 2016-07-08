/*
 * fft.c
 *
 * Code generation for function 'fft'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep_fft.h"
#include "fft.h"

/* Function Definitions */
void fft(const float x[57600], creal32_T y[65536])
{
  int i;
  float costab1q[16385];
  int k;
  static float costab[32769];
  static float sintab[32769];
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  float temp_re;
  int iDelta;
  int iDelta2;
  int iheight;
  float temp_im;
  float twid_re;
  float twid_im;
  for (i = 0; i < 65536; i++) {
    y[i].re = 0.0F;
    y[i].im = 0.0F;
  }

  costab1q[0] = 1.0F;
  for (k = 0; k < 8192; k++) {
    costab1q[k + 1] = (real32_T)cos(9.58738E-5F * ((float)k + 1.0F));
  }

  for (k = 0; k < 8191; k++) {
    costab1q[k + 8193] = (real32_T)sin(9.58738E-5F * (16384.0F - ((float)k +
      8193.0F)));
  }

  costab1q[16384] = 0.0F;
  costab[0] = 1.0F;
  sintab[0] = 0.0F;
  for (k = 0; k < 16384; k++) {
    costab[k + 1] = costab1q[k + 1];
    sintab[k + 1] = -costab1q[16383 - k];
  }

  for (k = 0; k < 16384; k++) {
    costab[k + 16385] = -costab1q[16383 - k];
    sintab[k + 16385] = -costab1q[k + 1];
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 57599; i++) {
    y[iy].re = x[ix];
    y[iy].im = 0.0F;
    iy = 65536;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy].re = x[ix];
  y[iy].im = 0.0F;
  for (i = 0; i < 65536; i += 2) {
    temp_re = y[i + 1].re;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = 0.0F;
    y[i].re += temp_re;
    y[i].im = 0.0F;
  }

  iDelta = 2;
  iDelta2 = 4;
  k = 16384;
  iheight = 65533;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = y[iy].re;
      temp_im = y[iy].im;
      y[i + iDelta].re = y[i].re - y[iy].re;
      y[i + iDelta].im = y[i].im - y[iy].im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    iy = 1;
    for (ix = k; ix < 32768; ix += k) {
      twid_re = costab[ix];
      twid_im = sintab[ix];
      i = iy;
      ju = iy + iheight;
      while (i < ju) {
        temp_re = twid_re * y[i + iDelta].re - twid_im * y[i + iDelta].im;
        temp_im = twid_re * y[i + iDelta].im + twid_im * y[i + iDelta].re;
        y[i + iDelta].re = y[i].re - temp_re;
        y[i + iDelta].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += iDelta2;
      }

      iy++;
    }

    k /= 2;
    iDelta = iDelta2;
    iDelta2 <<= 1;
    iheight -= iDelta;
  }
}

/* End of code generation (fft.c) */
