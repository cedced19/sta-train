/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controle_v3_data.c
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

#include "controle_v3.h"
#include "controle_v3_private.h"

/* Block parameters (default storage) */
P_controle_v3_T controle_v3_P = {
  /* Variable: Observer
   * Referenced by: '<Root>/Gain5'
   */
  {
    200.0,
    0.02,
    0.77970326741207219,
    256.50783876258947,

    { 1.0, 200.0 },
    -200.0,
    22.666666666666668
  },

  /* Variable: A
   * Referenced by: '<Root>/Gain3'
   */
  -30.0,

  /* Variable: B
   * Referenced by: '<Root>/Gain2'
   */
  4.0,

  /* Variable: C
   * Referenced by: '<Root>/Gain4'
   */
  7.5,

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

  /* Expression: 0
   * Referenced by: '<Root>/Integrator1'
   */
  0.0,

  /* Expression: 50
   * Referenced by: '<Root>/Saturation'
   */
  50.0,

  /* Expression: -50
   * Referenced by: '<Root>/Saturation'
   */
  -50.0
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
