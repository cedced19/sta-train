/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CONTROLADOR_types.h
 *
 * Code generated for Simulink model 'CONTROLADOR'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
 * C/C++ source code generated on : Thu Nov  4 05:56:59 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CONTROLADOR_types_h_
#define RTW_HEADER_CONTROLADOR_types_h_
#include "rtwtypes.h"

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
typedef struct P_CONTROLADOR_T_ P_CONTROLADOR_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_CONTROLADOR_T RT_MODEL_CONTROLADOR_T;

#endif                                 /* RTW_HEADER_CONTROLADOR_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
