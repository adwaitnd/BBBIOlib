/*
 * _coder_Prep_api.c
 *
 * Code generation for function 'Prep'
 *
 */

/* Include files */
#include "_coder_Prep_api.h"

/* Function Declarations */
static real32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *raw_data,
  const char_T *identifier))[192000];
static real32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[192000];
static real32_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *m_filter, const char_T *identifier))[100000];
static real32_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[100000];
static const mxArray *emlrt_marshallOut(const emxArray_real32_T *u);
static real32_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[192000];
static real32_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[100000];
static void emxInit_real32_T(const emlrtStack *sp, emxArray_real32_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxFree_real32_T(emxArray_real32_T **pEmxArray);

/* Function Definitions */
void Prep_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void Prep_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

void Prep_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  Prep_xil_terminate();
}

void Prep_api(const mxArray *prhs[2], const mxArray *plhs[1])
{
  emxArray_real32_T *prc_data;
  real32_T (*raw_data)[192000];
  real32_T (*m_filter)[100000];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real32_T(&st, &prc_data, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);

  /* Marshall function inputs */
  raw_data = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "raw_data");
  m_filter = c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "m_filter");

  /* Invoke the target function */
  Prep(*raw_data, *m_filter, prc_data);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(prc_data);
  prc_data->canFreeData = false;
  emxFree_real32_T(&prc_data);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static real32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *raw_data,
  const char_T *identifier))[192000]
{
  real32_T (*y)[192000];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = b_emlrt_marshallIn(sp, emlrtAlias(raw_data), &thisId);
  emlrtDestroyArray(&raw_data);
  return y;
}
  static real32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[192000]
{
  real32_T (*y)[192000];
  y = e_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static real32_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *m_filter, const char_T *identifier))[100000]
{
  real32_T (*y)[100000];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = d_emlrt_marshallIn(sp, emlrtAlias(m_filter), &thisId);
  emlrtDestroyArray(&m_filter);
  return y;
}
  static real32_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[100000]
{
  real32_T (*y)[100000];
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *emlrt_marshallOut(const emxArray_real32_T *u)
{
  const mxArray *y;
  static const int32_T iv0[2] = { 0, 0 };

  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateNumericArray(2, iv0, mxSINGLE_CLASS, mxREAL);
  mxSetData((mxArray *)m0, (void *)u->data);
  emlrtSetDimensions((mxArray *)m0, u->size, 2);
  emlrtAssign(&y, m0);
  return y;
}

static real32_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[192000]
{
  real32_T (*ret)[192000];
  int32_T iv1[2];
  int32_T i0;
  for (i0 = 0; i0 < 2; i0++) {
    iv1[i0] = 1 + 191999 * i0;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 2U, iv1);
  ret = (real32_T (*)[192000])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
  static real32_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[100000]
{
  real32_T (*ret)[100000];
  int32_T iv2[2];
  int32_T i1;
  for (i1 = 0; i1 < 2; i1++) {
    iv2[i1] = 1 + 99999 * i1;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single", false, 2U, iv2);
  ret = (real32_T (*)[100000])mxGetData(src);
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

/* End of code generation (_coder_Prep_api.c) */
