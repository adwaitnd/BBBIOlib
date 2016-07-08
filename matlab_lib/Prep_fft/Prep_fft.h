/*
 * Prep_fft.h
 *
 * Code generation for function 'Prep_fft'
 *
 */

#ifndef __PREP_FFT_H__
#define __PREP_FFT_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "Prep_fft_types.h"

/* Function Declarations */
extern void Prep_fft(const float raw_data[57600], float FS, float START_F, float
                     END_F, emxArray_real32_T *prc_data);

#endif

/* End of code generation (Prep_fft.h) */
