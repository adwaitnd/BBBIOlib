/*
 * fft.h
 *
 * Code generation for function 'fft'
 *
 */

#ifndef __FFT_H__
#define __FFT_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "Prep_fft_types.h"

/* Function Declarations */
extern void fft(const emxArray_real32_T *x, double varargin_1,
                emxArray_creal32_T *y);

#endif

/* End of code generation (fft.h) */
