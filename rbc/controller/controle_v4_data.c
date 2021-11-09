/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v4_data.c
 *
 * Code generated for Simulink model 'controle_v4'.
 *
 * Model version                  : 2.6
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Mon Nov  8 21:46:26 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controle_v4.h"
#include "controle_v4_private.h"

/* Block parameters (default storage) */
P_controle_v4_T controle_v4_P = {
  /* Variable: Ki
   * Referenced by: '<Root>/Gain'
   */
  0.021709343833047252,

  /* Variable: Ks
   * Referenced by: '<Root>/Gain6'
   */
  0.021709343833047252,

  /* Variable: Kx
   * Referenced by: '<Root>/Gain1'
   */
  -7.166666666666667,

  /* Expression: 0
   * Referenced by: '<Root>/Integrator'
   */
  0.0,

  /* Expression: 50
   * Referenced by: '<Root>/Saturation'
   */
  50.0,

  /* Expression: 0
   * Referenced by: '<Root>/Saturation'
   */
  0.0
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
