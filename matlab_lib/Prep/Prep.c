/*
 * Prep.c
 *
 * Code generation for function 'Prep'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "Prep_emxutil.h"
#include "abs.h"
#include "bsxfun.h"
#include "fft.h"
#include "linspace.h"

/* Function Declarations */
static int compute_nones(const boolean_T x[131073]);
static void eml_li_find(const boolean_T x[131073], emxArray_int32_T *y);

/* Function Definitions */
static int compute_nones(const boolean_T x[131073])
{
  int k;
  int i;
  k = 0;
  for (i = 0; i < 131073; i++) {
    if (x[i]) {
      k++;
    }
  }

  return k;
}

static void eml_li_find(const boolean_T x[131073], emxArray_int32_T *y)
{
  int j;
  int i;
  j = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = compute_nones(x);
  emxEnsureCapacity((emxArray__common *)y, j, (int)sizeof(int));
  j = 0;
  for (i = 0; i < 131073; i++) {
    if (x[i]) {
      y->data[j] = i + 1;
      j++;
    }
  }
}

void Prep(const float raw_data[192000], const float m_filter[100000],
          emxArray_real32_T *prc_data)
{
  static double f[131073];
  int i0;
  static creal32_T tmp_data[262144];
  static creal32_T Y_temp[262144];
  static creal32_T b_tmp_data[262144];
  static creal32_T b_Y_temp[262144];
  static float fv0[131073];
  boolean_T bv0[131073];
  emxArray_int32_T *r0;
  int loop_ub;

  /*  PREPROCESSING Summary of this function goes here */
  /*  Take the raw data, match filtered it and output in frequency domain */
  /* coder.inline('never'); */
  /*  Next power of 2 from length of y */
  linspace(f);
  for (i0 = 0; i0 < 131073; i0++) {
    f[i0] *= 96000.0;
  }

  /*  frequency band */
  fft(raw_data, tmp_data);
  b_fft(m_filter, Y_temp);
  for (i0 = 0; i0 < 262144; i0++) {
    if (tmp_data[i0].im == 0.0F) {
      b_tmp_data[i0].re = tmp_data[i0].re / 192000.0F;
      b_tmp_data[i0].im = 0.0F;
    } else if (tmp_data[i0].re == 0.0F) {
      b_tmp_data[i0].re = 0.0F;
      b_tmp_data[i0].im = tmp_data[i0].im / 192000.0F;
    } else {
      b_tmp_data[i0].re = tmp_data[i0].re / 192000.0F;
      b_tmp_data[i0].im = tmp_data[i0].im / 192000.0F;
    }

    b_Y_temp[i0].re = Y_temp[i0].re;
    b_Y_temp[i0].im = -Y_temp[i0].im;
  }

  bsxfun(b_tmp_data, b_Y_temp, tmp_data);
  b_abs(*(creal32_T (*)[131073])&tmp_data[0], fv0);
  for (i0 = 0; i0 < 131073; i0++) {
    bv0[i0] = ((20000.0 < f[i0]) && (f[i0] < 23000.0));
  }

  emxInit_int32_T(&r0, 2);
  eml_li_find(bv0, r0);
  i0 = prc_data->size[0] * prc_data->size[1];
  prc_data->size[0] = 1;
  prc_data->size[1] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)prc_data, i0, (int)sizeof(float));
  loop_ub = r0->size[0] * r0->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    prc_data->data[i0] = 2.0F * fv0[r0->data[i0] - 1];
  }

  emxFree_int32_T(&r0);
}

/* End of code generation (Prep.c) */
