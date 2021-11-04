/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CONTROLADOR_data.c
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

#include "CONTROLADOR.h"
#include "CONTROLADOR_private.h"

/* Block parameters (default storage) */
P_CONTROLADOR_T CONTROLADOR_P = {
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
