/*
 * Prep_emxAPI.h
 *
 * Code generation for function 'Prep_emxAPI'
 *
 */

#ifndef __PREP_EMXAPI_H__
#define __PREP_EMXAPI_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "Prep_types.h"

/* Function Declarations */
extern emxArray_real32_T *emxCreateND_real32_T(int numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapperND_real32_T(float *data, int
  numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows, int
  cols);
extern emxArray_real32_T *emxCreate_real32_T(int rows, int cols);
extern void emxDestroyArray_real32_T(emxArray_real32_T *emxArray);

#endif

/* End of code generation (Prep_emxAPI.h) */
