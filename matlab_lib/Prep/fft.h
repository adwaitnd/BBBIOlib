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
#include "Prep_types.h"

/* Function Declarations */
extern void b_fft(const float x[100000], creal32_T y[262144]);
extern void fft(const float x[192000], creal32_T y[262144]);

#endif

/* End of code generation (fft.h) */
