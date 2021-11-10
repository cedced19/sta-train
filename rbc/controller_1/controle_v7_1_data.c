/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v7_1_data.c
 *
 * Code generated for Simulink model 'controle_v7_1'.
 *
 * Model version                  : 2.10
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Nov  9 15:49:49 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controle_v7_1.h"
#include "controle_v7_1_private.h"

/* Block parameters (default storage) */
P_controle_v7_1_T controle_v7_1_P = {
  /* Variable: C
   * Referenced by: '<Root>/Gain1'
   */
  2.8975,

  /* Variable: Ki
   * Referenced by: '<Root>/Gain'
   */
  0.056193297238258617,

  /* Variable: Ks
   * Referenced by: '<Root>/Gain6'
   */
  0.056193297238258617,

  /* Variable: Kx
   * Referenced by: '<Root>/Gain1'
   */
  -2.6566666666666667,

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
