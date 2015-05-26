/*
 * _coder_Prep_api.h
 *
 * Code generation for function 'Prep'
 *
 */

#ifndef ___CODER_PREP_API_H__
#define ___CODER_PREP_API_H__
/* Include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_real32_T
#define struct_emxArray_real32_T
struct emxArray_real32_T
{
    real32_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
};
#endif /*struct_emxArray_real32_T*/
#ifndef typedef_emxArray_real32_T
#define typedef_emxArray_real32_T
typedef struct emxArray_real32_T emxArray_real32_T;
#endif /*typedef_emxArray_real32_T*/

/* Function Declarations */
extern void Prep_initialize(emlrtContext *aContext);
extern void Prep_terminate(void);
extern void Prep_atexit(void);
extern void Prep_api(const mxArray *prhs[5], const mxArray *plhs[1]);
extern void Prep(real32_T raw_data[192000], real32_T m_filter[100000], real32_T FS, real32_T START_F, real32_T END_F, emxArray_real32_T *prc_data);
extern void Prep_xil_terminate(void);

#endif
/* End of code generation (_coder_Prep_api.h) */
