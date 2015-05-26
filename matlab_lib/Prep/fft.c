/*
 * fft.c
 *
 * Code generation for function 'fft'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "fft.h"

/* Function Definitions */
void b_fft(const float x[100000], creal32_T y[262144])
{
  static creal32_T b_y1[262144];
  int i;
  static float costab1q[65537];
  int k;
  static float costab[131073];
  static float sintab[131073];
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iDelta;
  int iDelta2;
  int iheight;
  float twid_re;
  float twid_im;
  for (i = 0; i < 262144; i++) {
    b_y1[i].re = 0.0F;
    b_y1[i].im = 0.0F;
  }

  costab1q[0] = 1.0F;
  for (k = 0; k < 32768; k++) {
    costab1q[k + 1] = (real32_T)cos(2.39684505E-5F * ((float)k + 1.0F));
  }

  for (k = 0; k < 32767; k++) {
    costab1q[k + 32769] = (real32_T)sin(2.39684505E-5F * (65536.0F - ((float)k +
      32769.0F)));
  }

  costab1q[65536] = 0.0F;
  costab[0] = 1.0F;
  sintab[0] = 0.0F;
  for (k = 0; k < 65536; k++) {
    costab[k + 1] = costab1q[k + 1];
    sintab[k + 1] = -costab1q[65535 - k];
  }

  for (k = 0; k < 65536; k++) {
    costab[k + 65537] = -costab1q[65535 - k];
    sintab[k + 65537] = -costab1q[k + 1];
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 99999; i++) {
    b_y1[iy].re = x[ix];
    b_y1[iy].im = 0.0F;
    iy = 262144;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  b_y1[iy].re = x[ix];
  b_y1[iy].im = 0.0F;
  for (i = 0; i < 262144; i += 2) {
    temp_re = b_y1[i + 1].re;
    temp_im = b_y1[i + 1].im;
    b_y1[i + 1].re = b_y1[i].re - b_y1[i + 1].re;
    b_y1[i + 1].im = b_y1[i].im - b_y1[i + 1].im;
    b_y1[i].re += temp_re;
    b_y1[i].im += temp_im;
  }

  iDelta = 2;
  iDelta2 = 4;
  k = 65536;
  iheight = 262141;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = b_y1[iy].re;
      temp_im = b_y1[iy].im;
      b_y1[i + iDelta].re = b_y1[i].re - b_y1[iy].re;
      b_y1[i + iDelta].im = b_y1[i].im - b_y1[iy].im;
      b_y1[i].re += temp_re;
      b_y1[i].im += temp_im;
    }

    iy = 1;
    for (ix = k; ix < 131072; ix += k) {
      twid_re = costab[ix];
      twid_im = sintab[ix];
      i = iy;
      ju = iy + iheight;
      while (i < ju) {
        temp_re = twid_re * b_y1[i + iDelta].re - twid_im * b_y1[i + iDelta].im;
        temp_im = twid_re * b_y1[i + iDelta].im + twid_im * b_y1[i + iDelta].re;
        b_y1[i + iDelta].re = b_y1[i].re - temp_re;
        b_y1[i + iDelta].im = b_y1[i].im - temp_im;
        b_y1[i].re += temp_re;
        b_y1[i].im += temp_im;
        i += iDelta2;
      }

      iy++;
    }

    k /= 2;
    iDelta = iDelta2;
    iDelta2 <<= 1;
    iheight -= iDelta;
  }

  memcpy(&y[0], &b_y1[0], sizeof(creal32_T) << 18);
}

void fft(const float x[192000], creal32_T y[262144])
{
  static creal32_T b_y1[262144];
  int i;
  static float costab1q[65537];
  int k;
  static float costab[131073];
  static float sintab[131073];
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iDelta;
  int iDelta2;
  int iheight;
  float twid_re;
  float twid_im;
  for (i = 0; i < 262144; i++) {
    b_y1[i].re = 0.0F;
    b_y1[i].im = 0.0F;
  }

  costab1q[0] = 1.0F;
  for (k = 0; k < 32768; k++) {
    costab1q[k + 1] = (real32_T)cos(2.39684505E-5F * ((float)k + 1.0F));
  }

  for (k = 0; k < 32767; k++) {
    costab1q[k + 32769] = (real32_T)sin(2.39684505E-5F * (65536.0F - ((float)k +
      32769.0F)));
  }

  costab1q[65536] = 0.0F;
  costab[0] = 1.0F;
  sintab[0] = 0.0F;
  for (k = 0; k < 65536; k++) {
    costab[k + 1] = costab1q[k + 1];
    sintab[k + 1] = -costab1q[65535 - k];
  }

  for (k = 0; k < 65536; k++) {
    costab[k + 65537] = -costab1q[65535 - k];
    sintab[k + 65537] = -costab1q[k + 1];
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 191999; i++) {
    b_y1[iy].re = x[ix];
    b_y1[iy].im = 0.0F;
    iy = 262144;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  b_y1[iy].re = x[ix];
  b_y1[iy].im = 0.0F;
  for (i = 0; i < 262144; i += 2) {
    temp_re = b_y1[i + 1].re;
    temp_im = b_y1[i + 1].im;
    b_y1[i + 1].re = b_y1[i].re - b_y1[i + 1].re;
    b_y1[i + 1].im = b_y1[i].im - b_y1[i + 1].im;
    b_y1[i].re += temp_re;
    b_y1[i].im += temp_im;
  }

  iDelta = 2;
  iDelta2 = 4;
  k = 65536;
  iheight = 262141;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = b_y1[iy].re;
      temp_im = b_y1[iy].im;
      b_y1[i + iDelta].re = b_y1[i].re - b_y1[iy].re;
      b_y1[i + iDelta].im = b_y1[i].im - b_y1[iy].im;
      b_y1[i].re += temp_re;
      b_y1[i].im += temp_im;
    }

    iy = 1;
    for (ix = k; ix < 131072; ix += k) {
      twid_re = costab[ix];
      twid_im = sintab[ix];
      i = iy;
      ju = iy + iheight;
      while (i < ju) {
        temp_re = twid_re * b_y1[i + iDelta].re - twid_im * b_y1[i + iDelta].im;
        temp_im = twid_re * b_y1[i + iDelta].im + twid_im * b_y1[i + iDelta].re;
        b_y1[i + iDelta].re = b_y1[i].re - temp_re;
        b_y1[i + iDelta].im = b_y1[i].im - temp_im;
        b_y1[i].re += temp_re;
        b_y1[i].im += temp_im;
        i += iDelta2;
      }

      iy++;
    }

    k /= 2;
    iDelta = iDelta2;
    iDelta2 <<= 1;
    iheight -= iDelta;
  }

  memcpy(&y[0], &b_y1[0], sizeof(creal32_T) << 18);
}

/* End of code generation (fft.c) */
