/*
 * Prep.h
 *
 * Code generation for function 'Prep'
 *
 */

#ifndef __PREP_H__
#define __PREP_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "Prep_types.h"

/* Function Declarations */
extern void Prep(const float raw_data[192000], const float m_filter[100000],
                 emxArray_real32_T *prc_data);

#endif

/* End of code generation (Prep.h) */
