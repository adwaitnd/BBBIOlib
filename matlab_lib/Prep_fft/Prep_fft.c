/*
 * Prep_fft.c
 *
 * Code generation for function 'Prep_fft'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep_fft.h"
#include "Prep_fft_emxutil.h"
#include "abs.h"
#include "fft.h"
#include "linspace.h"

/* Function Declarations */
static int compute_nones(const boolean_T x[32769]);
static void eml_li_find(const boolean_T x[32769], emxArray_int32_T *y);

/* Function Definitions */
static int compute_nones(const boolean_T x[32769])
{
  int k;
  int i;
  k = 0;
  for (i = 0; i < 32769; i++) {
    if (x[i]) {
      k++;
    }
  }

  return k;
}

static void eml_li_find(const boolean_T x[32769], emxArray_int32_T *y)
{
  int j;
  int i;
  j = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = compute_nones(x);
  emxEnsureCapacity((emxArray__common *)y, j, (int)sizeof(int));
  j = 0;
  for (i = 0; i < 32769; i++) {
    if (x[i]) {
      y->data[j] = i + 1;
      j++;
    }
  }
}

void Prep_fft(const float raw_data[57600], float FS, float START_F, float END_F,
              emxArray_real32_T *prc_data)
{
  float y;
  static double dv0[32769];
  static float f[32769];
  int i;
  static creal32_T A[65536];
  static creal32_T b_A[32769];
  boolean_T b_START_F[32769];
  emxArray_int32_T *r0;
  int loop_ub;

  /*  PREPROCESSING Summary of this function goes here */
  /*  Take the raw data, take fft and output in frequency domain */
  /*  FS = 192000; */
  /*  Next power of 2 from length of y */
  y = FS / 2.0F;
  linspace(dv0);
  for (i = 0; i < 32769; i++) {
    f[i] = y * (float)dv0[i];
  }

  /*  frequency band */
  fft(raw_data, A);
  for (i = 0; i < 32769; i++) {
    if (A[i].im == 0.0F) {
      b_A[i].re = A[i].re / 57600.0F;
      b_A[i].im = 0.0F;
    } else if (A[i].re == 0.0F) {
      b_A[i].re = 0.0F;
      b_A[i].im = A[i].im / 57600.0F;
    } else {
      b_A[i].re = A[i].re / 57600.0F;
      b_A[i].im = A[i].im / 57600.0F;
    }

    b_START_F[i] = ((START_F < f[i]) && (f[i] < END_F));
  }

  b_abs(b_A, f);
  emxInit_int32_T(&r0, 2);
  eml_li_find(b_START_F, r0);
  i = prc_data->size[0];
  prc_data->size[0] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)prc_data, i, (int)sizeof(float));
  loop_ub = r0->size[1];
  for (i = 0; i < loop_ub; i++) {
    prc_data->data[i] = 2.0F * f[r0->data[r0->size[0] * i] - 1];
  }

  emxFree_int32_T(&r0);
}

/* End of code generation (Prep_fft.c) */
