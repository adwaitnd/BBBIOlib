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
#include "Prep_fft_emxutil.h"

/* Function Declarations */
static int div_s32(int numerator, int denominator);

/* Function Definitions */
static int div_s32(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator >= 0) {
      absNumerator = (unsigned int)numerator;
    } else {
      absNumerator = (unsigned int)-numerator;
    }

    if (denominator >= 0) {
      absDenominator = (unsigned int)denominator;
    } else {
      absDenominator = (unsigned int)-denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    absNumerator /= absDenominator;
    if (quotientNeedsNegation) {
      quotient = -(int)absNumerator;
    } else {
      quotient = (int)absNumerator;
    }
  }

  return quotient;
}

void fft(const emxArray_real32_T *x, double varargin_1, emxArray_creal32_T *y)
{
  int ju;
  int nd2;
  int u1;
  int ixDelta;
  emxArray_real32_T *costab1q;
  int nRowsD2;
  int nRowsD4;
  int lastChan;
  float e;
  int k;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  int n;
  int ix;
  int chanStart;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iDelta2;
  int iheight;
  int ihi;
  float twid_im;
  ju = y->size[0];
  y->size[0] = (int)varargin_1;
  emxEnsureCapacity((emxArray__common *)y, ju, (int)sizeof(creal32_T));
  if ((int)varargin_1 > x->size[0]) {
    ju = y->size[0];
    y->size[0] = (int)varargin_1;
    emxEnsureCapacity((emxArray__common *)y, ju, (int)sizeof(creal32_T));
    nd2 = (int)varargin_1;
    for (ju = 0; ju < nd2; ju++) {
      y->data[ju].re = 0.0F;
      y->data[ju].im = 0.0F;
    }
  }

  if (x->size[0] == 0) {
  } else {
    nd2 = (int)varargin_1;
    u1 = x->size[0];
    if (nd2 <= u1) {
      u1 = nd2;
    }

    nd2 = (x->size[0] - u1) + 1;
    if (1 >= nd2) {
      ixDelta = 1;
    } else {
      ixDelta = nd2;
    }

    emxInit_real32_T(&costab1q, 2);
    ju = (int)varargin_1;
    nRowsD2 = ju / 2;
    nRowsD4 = nRowsD2 / 2;
    lastChan = (int)varargin_1 * (div_s32(x->size[0], x->size[0]) - 1);
    e = 6.28318548F / (float)(int)varargin_1;
    ju = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = nRowsD4 + 1;
    emxEnsureCapacity((emxArray__common *)costab1q, ju, (int)sizeof(float));
    costab1q->data[0] = 1.0F;
    nd2 = nRowsD4 / 2;
    for (k = 1; k <= nd2; k++) {
      costab1q->data[k] = (real32_T)cos(e * (float)k);
    }

    for (k = nd2 + 1; k < nRowsD4; k++) {
      costab1q->data[k] = (real32_T)sin(e * (float)(nRowsD4 - k));
    }

    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    costab1q->data[nRowsD4] = 0.0F;
    n = costab1q->size[1] - 1;
    nd2 = (costab1q->size[1] - 1) << 1;
    ju = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nd2 + 1;
    emxEnsureCapacity((emxArray__common *)costab, ju, (int)sizeof(float));
    ju = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nd2 + 1;
    emxEnsureCapacity((emxArray__common *)sintab, ju, (int)sizeof(float));
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    for (k = 1; k <= n; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[n - k];
    }

    for (k = costab1q->size[1]; k <= nd2; k++) {
      costab->data[k] = -costab1q->data[nd2 - k];
      sintab->data[k] = -costab1q->data[k - n];
    }

    emxFree_real32_T(&costab1q);
    ix = 0;
    chanStart = 0;
    while (((int)varargin_1 > 0) && (chanStart <= lastChan)) {
      ju = 0;
      nd2 = chanStart;
      for (i = 1; i < u1; i++) {
        y->data[nd2].re = x->data[ix];
        y->data[nd2].im = 0.0F;
        n = (int)varargin_1;
        tst = true;
        while (tst) {
          n >>= 1;
          ju ^= n;
          tst = ((ju & n) == 0);
        }

        nd2 = chanStart + ju;
        ix++;
      }

      y->data[nd2].re = x->data[ix];
      y->data[nd2].im = 0.0F;
      ix += ixDelta;
      nd2 = (chanStart + (int)varargin_1) - 2;
      if ((int)varargin_1 > 1) {
        for (i = chanStart; i <= nd2; i += 2) {
          temp_re = y->data[i + 1].re;
          temp_im = y->data[i + 1].im;
          y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
          y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }
      }

      n = 2;
      iDelta2 = 4;
      k = nRowsD4;
      iheight = 1 + ((nRowsD4 - 1) << 2);
      while (k > 0) {
        i = chanStart;
        ihi = chanStart + iheight;
        while (i < ihi) {
          nd2 = i + n;
          temp_re = y->data[nd2].re;
          temp_im = y->data[nd2].im;
          y->data[i + n].re = y->data[i].re - y->data[nd2].re;
          y->data[i + n].im = y->data[i].im - y->data[nd2].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
          i += iDelta2;
        }

        nd2 = chanStart + 1;
        for (ju = k; ju < nRowsD2; ju += k) {
          e = costab->data[ju];
          twid_im = sintab->data[ju];
          i = nd2;
          ihi = nd2 + iheight;
          while (i < ihi) {
            temp_re = e * y->data[i + n].re - twid_im * y->data[i + n].im;
            temp_im = e * y->data[i + n].im + twid_im * y->data[i + n].re;
            y->data[i + n].re = y->data[i].re - temp_re;
            y->data[i + n].im = y->data[i].im - temp_im;
            y->data[i].re += temp_re;
            y->data[i].im += temp_im;
            i += iDelta2;
          }

          nd2++;
        }

        k /= 2;
        n = iDelta2;
        iDelta2 <<= 1;
        iheight -= n;
      }

      chanStart += (int)varargin_1;
    }

    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

/* End of code generation (fft.c) */
