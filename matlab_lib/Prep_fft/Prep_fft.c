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
#include "fft.h"

/* Function Declarations */
static float rt_hypotf_snf(float u0, float u1);
static double rt_powd_snf(double u0, double u1);

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

static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d0;
  double d1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d0 = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d0 == 1.0) {
        y = rtNaN;
      } else if (d0 > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

void Prep_fft(const emxArray_real32_T *raw_data, float FS, float START_F, float
              END_F, emxArray_real32_T *prc_data)
{
  int j;
  double delta1;
  double p;
  emxArray_real_T *y;
  double NFFT;
  float b_y;
  int i;
  emxArray_real32_T *f;
  emxArray_creal32_T *tmp_data;
  float tmp_data_re;
  float tmp_data_im;
  emxArray_real_T *r0;
  emxArray_creal32_T *x;
  emxArray_int32_T *r1;
  emxArray_real32_T *b_tmp_data;
  emxArray_int32_T *r2;
  emxArray_boolean_T *b_x;
  int n;
  emxArray_int32_T *r3;

  /*  PREPROCESSING Summary of this function goes here */
  /*  Take the raw data, take fft and output in frequency domain */
  /*  FS = 192000; */
  delta1 = frexp(raw_data->size[0], &j);
  p = j;
  if (delta1 == 0.5) {
    p = (double)j - 1.0;
  }

  b_emxInit_real_T(&y, 2);
  NFFT = rt_powd_snf(2.0, p);

  /*  Next power of 2 from length of y */
  b_y = FS / 2.0F;
  delta1 = floor(NFFT / 2.0 + 1.0);
  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)delta1;
  emxEnsureCapacity((emxArray__common *)y, i, (int)sizeof(double));
  y->data[(int)delta1 - 1] = 1.0;
  if (y->size[1] >= 2) {
    y->data[0] = 0.0;
    if (y->size[1] >= 3) {
      delta1 = 1.0 / ((double)y->size[1] - 1.0);
      i = y->size[1];
      for (j = 0; j <= i - 3; j++) {
        y->data[j + 1] = (1.0 + (double)j) * delta1;
      }
    }
  }

  emxInit_real32_T(&f, 2);
  i = f->size[0] * f->size[1];
  f->size[0] = 1;
  f->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)f, i, (int)sizeof(float));
  j = y->size[0] * y->size[1];
  for (i = 0; i < j; i++) {
    f->data[i] = b_y * (float)y->data[i];
  }

  emxFree_real_T(&y);
  emxInit_creal32_T(&tmp_data, 1);

  /*  frequency band */
  fft(raw_data, NFFT, tmp_data);
  b_y = (float)raw_data->size[0];
  i = tmp_data->size[0];
  emxEnsureCapacity((emxArray__common *)tmp_data, i, (int)sizeof(creal32_T));
  j = tmp_data->size[0];
  for (i = 0; i < j; i++) {
    tmp_data_re = tmp_data->data[i].re;
    tmp_data_im = tmp_data->data[i].im;
    if (tmp_data_im == 0.0F) {
      tmp_data->data[i].re = tmp_data_re / b_y;
      tmp_data->data[i].im = 0.0F;
    } else if (tmp_data_re == 0.0F) {
      tmp_data->data[i].re = 0.0F;
      tmp_data->data[i].im = tmp_data_im / b_y;
    } else {
      tmp_data->data[i].re = tmp_data_re / b_y;
      tmp_data->data[i].im = tmp_data_im / b_y;
    }
  }

  emxInit_real_T(&r0, 1);
  p = NFFT / 2.0;
  i = r0->size[0];
  r0->size[0] = (int)floor(p) + 1;
  emxEnsureCapacity((emxArray__common *)r0, i, (int)sizeof(double));
  j = (int)floor(p);
  for (i = 0; i <= j; i++) {
    r0->data[i] = i;
  }

  emxInit_creal32_T(&x, 1);
  p = NFFT / 2.0;
  i = x->size[0];
  x->size[0] = (int)floor(p) + 1;
  emxEnsureCapacity((emxArray__common *)x, i, (int)sizeof(creal32_T));
  j = (int)floor(p);
  for (i = 0; i <= j; i++) {
    x->data[i] = tmp_data->data[i];
  }

  emxFree_creal32_T(&tmp_data);
  b_emxInit_int32_T(&r1, 1);
  i = r1->size[0];
  r1->size[0] = r0->size[0];
  emxEnsureCapacity((emxArray__common *)r1, i, (int)sizeof(int));
  j = r0->size[0];
  for (i = 0; i < j; i++) {
    r1->data[i] = (int)(1.0 + r0->data[i]);
  }

  b_emxInit_real32_T(&b_tmp_data, 1);
  b_emxInit_int32_T(&r2, 1);
  j = r1->size[0];
  i = b_tmp_data->size[0];
  b_tmp_data->size[0] = j;
  emxEnsureCapacity((emxArray__common *)b_tmp_data, i, (int)sizeof(float));
  i = r2->size[0];
  r2->size[0] = r0->size[0];
  emxEnsureCapacity((emxArray__common *)r2, i, (int)sizeof(int));
  j = r0->size[0];
  emxFree_int32_T(&r1);
  for (i = 0; i < j; i++) {
    r2->data[i] = (int)(1.0 + r0->data[i]);
  }

  emxFree_real_T(&r0);
  i = r2->size[0];
  j = 0;
  emxFree_int32_T(&r2);
  while (j <= i - 1) {
    b_tmp_data->data[j] = rt_hypotf_snf(x->data[j].re, x->data[j].im);
    j++;
  }

  emxFree_creal32_T(&x);
  i = b_tmp_data->size[0];
  emxEnsureCapacity((emxArray__common *)b_tmp_data, i, (int)sizeof(float));
  j = b_tmp_data->size[0];
  for (i = 0; i < j; i++) {
    b_tmp_data->data[i] *= 2.0F;
  }

  emxInit_boolean_T(&b_x, 2);
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = f->size[1];
  emxEnsureCapacity((emxArray__common *)b_x, i, (int)sizeof(boolean_T));
  j = f->size[0] * f->size[1];
  for (i = 0; i < j; i++) {
    b_x->data[i] = ((START_F < f->data[i]) && (f->data[i] < END_F));
  }

  emxFree_real32_T(&f);
  n = b_x->size[1];
  j = 0;
  for (i = 1; i <= n; i++) {
    if (b_x->data[i - 1]) {
      j++;
    }
  }

  emxInit_int32_T(&r3, 2);
  i = r3->size[0] * r3->size[1];
  r3->size[0] = 1;
  r3->size[1] = j;
  emxEnsureCapacity((emxArray__common *)r3, i, (int)sizeof(int));
  j = 0;
  for (i = 1; i <= n; i++) {
    if (b_x->data[i - 1]) {
      r3->data[j] = i;
      j++;
    }
  }

  emxFree_boolean_T(&b_x);
  i = prc_data->size[0];
  prc_data->size[0] = r3->size[1];
  emxEnsureCapacity((emxArray__common *)prc_data, i, (int)sizeof(float));
  j = r3->size[1];
  for (i = 0; i < j; i++) {
    prc_data->data[i] = b_tmp_data->data[r3->data[r3->size[0] * i] - 1];
  }

  emxFree_int32_T(&r3);
  emxFree_real32_T(&b_tmp_data);
}

/* End of code generation (Prep_fft.c) */
