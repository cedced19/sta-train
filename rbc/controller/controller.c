/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controller.c
 *
 * Code generated for Simulink model 'controller'.
 *
 * Model version                  : 2.4
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Thu Nov  4 16:05:23 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controller.h"
#include "controller_private.h"

/* Block signals (default storage) */
B_controller_T controller_B;

/* Continuous states */
X_controller_T controller_X;

/* External inputs (root inport signals with default storage) */
ExtU_controller_T controller_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_controller_T controller_Y;

/* Real-time model */
static RT_MODEL_controller_T controller_M_;
RT_MODEL_controller_T *const controller_M = &controller_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  controller_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void controller_step(void)
{
  real_T Dfrenage;
  real_T rtb_Sum1;
  real_T x;
  if (rtmIsMajorTimeStep(controller_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&controller_M->solverInfo,
                          ((controller_M->Timing.clockTick0+1)*
      controller_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(controller_M)) {
    controller_M->Timing.t[0] = rtsiGetT(&controller_M->solverInfo);
  }

  /* Sum: '<Root>/Sum1' incorporates:
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain1'
   *  Integrator: '<Root>/Integrator'
   *  Integrator: '<Root>/Integrator1'
   */
  rtb_Sum1 = 0.021709343833047252 * controller_X.Integrator_CSTATE -
    -7.166666666666667 * controller_X.Integrator1_CSTATE;

  /* Saturate: '<Root>/Saturation' */
  if (rtb_Sum1 > 50.0) {
    /* Saturate: '<Root>/Saturation' */
    controller_Y.Vitesse_Envoyee = 50.0;
  } else if (rtb_Sum1 < -50.0) {
    /* Saturate: '<Root>/Saturation' */
    controller_Y.Vitesse_Envoyee = -50.0;
  } else {
    /* Saturate: '<Root>/Saturation' */
    controller_Y.Vitesse_Envoyee = rtb_Sum1;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Inport: '<Root>/Position '
   *  Inport: '<Root>/Position_2'
   *  Inport: '<Root>/Vitesse_Consigne'
   *  Inport: '<Root>/Vitesse_Reelle'
   */
  x = controller_U.Position - controller_U.Position_2;
  Dfrenage = controller_U.Vitesse_Reelle * controller_U.Vitesse_Reelle * 0.03 +
    0.28 * controller_U.Vitesse_Reelle;
  if (x <= 20.0) {
    x = 0.0;
  } else if ((20.0 < x) && (x <= Dfrenage + 20.0)) {
    x = (x * x - 400.0) * controller_U.Vitesse_Consigne / ((Dfrenage + 20.0) *
      (Dfrenage + 20.0));
  } else {
    x = controller_U.Vitesse_Consigne;
  }

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Inport: '<Root>/Light'
   */
  if (!(controller_U.Light == 0.0)) {
    if (controller_U.Light == 1.0) {
      x *= 0.2;
    } else {
      x = 0.0;
    }
  }

  /* End of MATLAB Function: '<S2>/MATLAB Function' */

  /* Sum: '<Root>/Sum' incorporates:
   *  Gain: '<Root>/Gain6'
   *  Inport: '<Root>/Vitesse_Reelle'
   *  Sum: '<Root>/Sum4'
   */
  controller_B.Sum = ((controller_Y.Vitesse_Envoyee - rtb_Sum1) *
                      0.021709343833047252 + x) - controller_U.Vitesse_Reelle;

  /* Sum: '<Root>/Sum2' incorporates:
   *  Gain: '<Root>/Gain2'
   *  Gain: '<Root>/Gain3'
   *  Gain: '<Root>/Gain4'
   *  Gain: '<Root>/Gain5'
   *  Inport: '<Root>/Vitesse_Reelle'
   *  Integrator: '<Root>/Integrator1'
   *  Sum: '<Root>/Sum3'
   */
  controller_B.Sum2 = ((controller_U.Vitesse_Reelle - 7.5 *
                        controller_X.Integrator1_CSTATE) * 22.666666666666668 +
                       4.0 * controller_Y.Vitesse_Envoyee) + -30.0 *
    controller_X.Integrator1_CSTATE;
  if (rtmIsMajorTimeStep(controller_M)) {
    rt_ertODEUpdateContinuousStates(&controller_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++controller_M->Timing.clockTick0;
    controller_M->Timing.t[0] = rtsiGetSolverStopTime(&controller_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.034s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.034, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      controller_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void controller_derivatives(void)
{
  XDot_controller_T *_rtXdot;
  _rtXdot = ((XDot_controller_T *) controller_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  _rtXdot->Integrator_CSTATE = controller_B.Sum;

  /* Derivatives for Integrator: '<Root>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = controller_B.Sum2;
}

/* Model initialize function */
void controller_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&controller_M->solverInfo,
                          &controller_M->Timing.simTimeStep);
    rtsiSetTPtr(&controller_M->solverInfo, &rtmGetTPtr(controller_M));
    rtsiSetStepSizePtr(&controller_M->solverInfo,
                       &controller_M->Timing.stepSize0);
    rtsiSetdXPtr(&controller_M->solverInfo, &controller_M->derivs);
    rtsiSetContStatesPtr(&controller_M->solverInfo, (real_T **)
                         &controller_M->contStates);
    rtsiSetNumContStatesPtr(&controller_M->solverInfo,
      &controller_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&controller_M->solverInfo,
      &controller_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&controller_M->solverInfo,
      &controller_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&controller_M->solverInfo,
      &controller_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&controller_M->solverInfo, (&rtmGetErrorStatus
      (controller_M)));
    rtsiSetRTModelPtr(&controller_M->solverInfo, controller_M);
  }

  rtsiSetSimTimeStep(&controller_M->solverInfo, MAJOR_TIME_STEP);
  controller_M->intgData.f[0] = controller_M->odeF[0];
  controller_M->contStates = ((X_controller_T *) &controller_X);
  rtsiSetSolverData(&controller_M->solverInfo, (void *)&controller_M->intgData);
  rtsiSetSolverName(&controller_M->solverInfo,"ode1");
  rtmSetTPtr(controller_M, &controller_M->Timing.tArray[0]);
  controller_M->Timing.stepSize0 = 0.034;

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  controller_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<Root>/Integrator1' */
  controller_X.Integrator1_CSTATE = 0.0;
}

/* Model terminate function */
void controller_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
