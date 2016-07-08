/*
 * _coder_Prep_fft_api.c
 *
 * Code generation for function 'Prep_fft'
 *
 */

/* Include files */
#include "_coder_Prep_fft_api.h"

/* Function Declarations */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *raw_data,
  const char_T *identifier, emxArray_real32_T *y);
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real32_T *y);
static real32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *FS,
  const char_T *identifier);
static real32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *emlrt_marshallOut(const emxArray_real32_T *u);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real32_T *ret);
static real32_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static void emxInit_real32_T(const emlrtStack *sp, emxArray_real32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxFree_real32_T(emxArray_real32_T **pEmxArray);

/* Function Definitions */
void Prep_fft_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void Prep_fft_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

void Prep_fft_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  Prep_fft_xil_terminate();
}

void Prep_fft_api(const mxArray *prhs[4], const mxArray *plhs[1])
{
  emxArray_real32_T *raw_data;
  emxArray_real32_T *prc_data;
  real32_T FS;
  real32_T START_F;
  real32_T END_F;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real32_T(&st, &raw_data, 1, true);
  emxInit_real32_T(&st, &prc_data, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "raw_data", raw_data);
  FS = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "FS");
  START_F = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "START_F");
  END_F = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "END_F");

  /* Invoke the target function */
  Prep_fft(raw_data, FS, START_F, END_F, prc_data);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(prc_data);
  prc_data->canFreeData = false;
  emxFree_real32_T(&prc_data);
  raw_data->canFreeData = false;
  emxFree_real32_T(&raw_data);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *raw_data,
  const char_T *identifier, emxArray_real32_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  b_emlrt_marshallIn(sp, emlrtAlias(raw_data), &thisId, y);
  emlrtDestroyArray(&raw_data);
}

static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real32_T *y)
{
  e_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static real32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *FS,
  const char_T *identifier)
{
  real32_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = d_emlrt_marshallIn(sp, emlrtAlias(FS), &thisId);
  emlrtDestroyArray(&FS);
  return y;
}

static real32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real32_T y;
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *emlrt_marshallOut(const emxArray_real32_T *u)
{
  const mxArray *y;
  static const int32_T iv0[1] = { 0 };

  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateNumericArray(1, iv0, mxSINGLE_CLASS, mxREAL);
  mxSetData((mxArray *)m0, (void *)u->data);
  emlrtSetDimensions((mxArray *)m0, u->size, 1);
  emlrtAssign(&y, m0);
  return y;
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real32_T *ret)
{
  int32_T iv1[1];
  boolean_T bv0[1];
  int32_T iv2[1];
  iv1[0] = -1;
  bv0[0] = true;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "single", false, 1U, iv1, bv0, iv2);
  ret->size[0] = iv2[0];
  ret->allocatedSize = ret->size[0];
  ret->data = (real32_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static real32_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  real32_T ret;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 0U, 0);
  ret = *(real32_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static void emxInit_real32_T(const emlrtStack *sp, emxArray_real32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real32_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real32_T *)emlrtMallocMex(sizeof(emxArray_real32_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real32_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real32_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxFree_real32_T(emxArray_real32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real32_T *)NULL) {
    if (((*pEmxArray)->data != (real32_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((void *)(*pEmxArray)->data);
    }

    emlrtFreeMex((void *)(*pEmxArray)->size);
    emlrtFreeMex((void *)*pEmxArray);
    *pEmxArray = (emxArray_real32_T *)NULL;
  }
}

/* End of code generation (_coder_Prep_fft_api.c) */
