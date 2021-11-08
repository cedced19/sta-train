/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v3_types.h
 *
 * Code generated for Simulink model 'controle_v3'.
 *
 * Model version                  : 2.1
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Mon Nov  8 11:19:10 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_controle_v3_types_h_
#define RTW_HEADER_controle_v3_types_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"

/* Model Code Variants */
#ifndef DEFINED_TYPEDEF_FOR_struct_ALRDS3SSLkjlF2CmbmN2xD_
#define DEFINED_TYPEDEF_FOR_struct_ALRDS3SSLkjlF2CmbmN2xD_

typedef struct {
  real_T wd;
  real_T PO;
  real_T zeta;
  real_T wn;
  real_T poly[2];
  real_T poles;
  real_T L;
} struct_ALRDS3SSLkjlF2CmbmN2xD;

#endif

/* Parameters (default storage) */
typedef struct P_controle_v3_T_ P_controle_v3_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_controle_v3_T RT_MODEL_controle_v3_T;

#endif                                 /* RTW_HEADER_controle_v3_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
